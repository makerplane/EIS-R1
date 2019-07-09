/*  Copyright (c) 2019 Phil Birkelbach
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* This program is the MakerPlane Engine Information System firmware */

#include <SPI.h>
#include <canfix.h>
#include <can.h>
#include "FRAM.h"
#include "config.h"
#include "analogs.h"

/* Comment this line to remove serial debugging code */
#define DEBUG_SERIAL 0x01

/* Pin definitions */
#define CAN_CS 34
#define FLASH_CS 42
#define FLASH_HOLD 46
#define FLASH_WP 44

#define STATE_ANALOG 0x01
#define STATE_TACH   0x02
#define STATE_END    0x03

/* Status Errors */
#define STAT_ERR_CONFIG 0x01

/* Job Indication Flags */
#define JOB_RECONFIG 0x01
//#define JOB_ 0x02
//#define JOB_ 0x04
//#define JOB_ 0x08
//#define JOB_ 0x10
//#define JOB_ 0x20
//#define JOB_ 0x40
//#define JOB_ 0x80

#define SET_FLAG(p,n) ((p) |= (n))
#define CLR_FLAG(p,n) ((p) &= (n))
#define CHK_FLAG(p,n) ((p) && (n))

/* The CanFix class represents our CAN-FIX connection */
CanFix *cf;
FRAM_SPI *flash;
Config cfg;
Analog analogs[11];
byte job_flags = 0x00;

uint16_t status = 0x0000;
volatile long last_tach_time1 = 0;
volatile long duration1 = 0;
volatile long last_tach_time2 = 0;
volatile long duration2 = 0;
volatile long duration3 = 0;
volatile long duration4 = 0;
volatile long fuel_counter1 = 0;
volatile long fuel_counter2 = 0;


/* Interrupt Service Routines */
void isr1(void) {
    long x;
    x = micros();
    duration1 = x - last_tach_time1;
    last_tach_time1 = x;
}

void isr2(void) {
    long x;
    x = micros();
    duration2 = x - last_tach_time2;
    last_tach_time2 = x;
}

void isr3(void) {
    fuel_counter1++;
}

void isr4(void) {
    fuel_counter2++;
}


/* CAN-FIX Callback routines */

void report_callback(void) {
    ;
}

byte config_callback(word key, byte *data) {
    uint8_t x;
    x = cfg.writeConfig(key, data);
    if(x) { // Success
        SET_FLAG(job_flags, JOB_RECONFIG);
        return 0x00;
    }
    return 0x01;  // Probably key not found
}

byte query_callback(word key, byte *data, byte *len) {
    *len = cfg.readConfig(key, data);
    if(len) return 0x00;
    return 0x01;
}


/* This function reads the configuration and writes the data to
   The proper place in all the objects */
void configure(void) {
    analogs[0].configure(10, &cfg, ANALOG_RESISTANCE);
    analogs[1].configure(32, &cfg, ANALOG_RESISTANCE);
    analogs[2].configure(54, &cfg, ANALOG_RESISTANCE);
    analogs[3].configure(76, &cfg, ANALOG_RESISTANCE);
    analogs[4].configure(98, &cfg, ANALOG_RESISTANCE);
    analogs[5].configure(120, &cfg, ANALOG_VOLTAGE);
    analogs[6].configure(142, &cfg, ANALOG_VOLTAGE);
    analogs[7].configure(164, &cfg, ANALOG_VOLTAGE);
    analogs[8].configure(186, &cfg, ANALOG_VOLTAGE);
    analogs[9].configure(208, &cfg, ANALOG_VOLTAGE);
    analogs[10].configure(230, &cfg, ANALOG_VOLTAGE);
}

/* This function checks the duration from the ISRs for the Tachometer inputs,
   calculates the RPMs and decides what to do with them */
void send_tachometers(void) {
    int k, rpm1, rpm2, rpm;
    unsigned int sel1, sel2;
    long x;
    CFParameter p;
    // Calculate the first tach reading
    if(duration1 != 0) {
        x = micros();
        // If it's been longer than a second then we're not turning
        if(x - last_tach_time1 > 5e5) {
            last_tach_time1 = x;
            duration1 = 0;
            rpm1 = 0;
        } else {
            // Read the pulses / revolution from the configuration
            cfg.readConfig(253, (uint8_t *)&k);
            noInterrupts();
            rpm1 = (int)((60e6 / duration1 / k) / 10) * 10;
            interrupts();
        }
    } else {
        rpm1 = 0;
    }
    // Calculate the second tach reading
    if(duration2 != 0) {
        x = micros();
        // If it's been longer than a second then we're not turning
        if(x - last_tach_time2 > 5e5) {
            last_tach_time2 = x;
            duration2 = 0;
            rpm2 = 0;
        } else {
            // Read the pulses / revolution from the configuration
            cfg.readConfig(269, (uint8_t *)&k);
            noInterrupts();
            rpm2 = (int)((60e6 / duration2 / k) / 10) * 10;
            interrupts();
        }
    } else {
        rpm2 = 0;
    }
    cfg.readConfig(252, (uint8_t *)&sel1);
    cfg.readConfig(268, (uint8_t *)&sel2);

    // These don't change
    p.index = 0;
    p.length = 2;

    // This would indicate redundant inputs for the same parameter
    if(sel1 == sel2) {
        p.type = sel1 >> 4;  // Retrieve the CAN-ID from the selection
        rpm = rpm1 > rpm2 ? rpm1 : rpm2; // High select
        p.data[0] = rpm;
        p.data[1] = rpm>>8;
        if(abs(rpm1 - rpm2) > 50) { // Deviation gives us quality of the data
            p.fcb = FCB_QUALITY;
        } else {
            p.fcb = 0x00;
        }
        cf->sendParam(p);
    } else {
        p.fcb = 0x00;
        p.type = sel1 >> 4;
        p.data[0] = rpm1;
        p.data[1] = rpm1>>8;
        cf->sendParam(p);

        p.type = sel2 >> 4;
        p.data[0] = rpm2;
        p.data[1] = rpm2>>8;
        cf->sendParam(p);
    }

}

void setup() {
    uint8_t result;
    long now, last = 0;

    pinMode(LED_BUILTIN, OUTPUT);

    SPI.setBitOrder(MSBFIRST);
    //SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.begin();
#ifdef DEBUG_SERIAL
    Serial.begin(115200);
#endif
    /* The constructor takes the pin that slave select for the MCP2515
     * is connected to and our CAN-FIX device type.  The Device type
     * will be used as the node id if there is nothing in the EEPROM */
    cf = new CanFix(CAN_CS, 0x40);
    /* This sets the model number and firmware version of our device.
     * This is what will be returned when the Node ID is requested */
    cf->setModel(0x000001);
    cf->setFwVersion(0x01);
    cf->set_report_callback(report_callback);
    cf->set_config_callback(config_callback);
    cf->set_query_callback(query_callback);

    /* Set up Interrupts */
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(18, INPUT);
    pinMode(19, INPUT);
    attachInterrupt(digitalPinToInterrupt(2), isr1, RISING);
    attachInterrupt(digitalPinToInterrupt(3), isr2, RISING);
    attachInterrupt(digitalPinToInterrupt(18), isr3, RISING);
    attachInterrupt(digitalPinToInterrupt(19), isr4, RISING);

    /* Instantiate the FRAM object */
    flash = new FRAM_SPI(FLASH_CS, FLASH_HOLD, FLASH_WP);
    /* The FRAM object sets and internal status if it cannot talk
     * to the chip */
    result = flash->getStatus();
    if(result) status |= STAT_ERR_CONFIG;

    /* If we have a bad status we lock into a loop that just sends the status every 2 seconds */
    if(status) {
        while(true) {
            now = millis();
            if(now - last > 2000) {
                cf->sendStatus(0x0000, (byte *)&status, 2);
                last = now;
            }
        }
    } else {
        cf->sendStatus(0x0000, (byte *)&status, 2);
    }
#ifdef DEBUG_SERIAL
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
    }
    Serial.println("Starting...");
#endif
    cfg.setFlash(flash);

    // Resistor Analogs
    analogs[0].input_pin = A10;
    analogs[1].input_pin = A7;
    analogs[2].input_pin = A6;
    analogs[3].input_pin = A5;
    analogs[4].input_pin = A1;
    // Voltage Analogs
    analogs[5].input_pin = A0;
    analogs[6].input_pin = A3;
    analogs[7].input_pin = A4;
    analogs[8].input_pin = A5;
    analogs[9].input_pin = A8;
    analogs[10].input_pin = A9;
    configure();
}


void loop() {
    byte n;
    static byte cycle = 0;
    static int state = STATE_ANALOG;
    static long next_250 = 250;
    static int index = 0;
    long now;
    static CFParameter p;
//    unsigned long start, end;

    now = millis();

    if(state == STATE_ANALOG) {
        analogs[index++].read();
        if(index == 11) {
            index = 0;
            state++;
        }
    } else if(state == STATE_TACH) {
        state++; // Calculate TACH
    } else if(state == STATE_END) {
        ;
    }

    if(now > next_250) {
        next_250 += 250;
        if(cycle % 4 == 0) {
            send_tachometers();
        }
        if(cycle % 2 == 0) {
            for(n=0;n<11;n++) {
                if(analogs[n].pid) { // pid is non zero if good
                    p.type = analogs[n].pid;
                    p.index = analogs[n].index;
                    p.setMetaData(0x00);
                    p.data[0] = analogs[n].value;
                    p.data[1] = analogs[n].value >> 8;
                    p.length = 2;
                    p.setFlags( analogs[n].quality );
                    cf->sendParam(p);
                }
            }
        }
        // Every second offset by 1 cycle
        if(cycle % 8 == 1) {
            cf->sendStatus(0x0000, (byte *)&status, 2);
        }
        // Every Two seconds
        if(cycle % 16 == 0) {
            if(CHK_FLAG(job_flags, JOB_RECONFIG)) {
                // This takes about 8mSec so incoming messages can be lost
                configure();
                CLR_FLAG(job_flags, JOB_RECONFIG);
            }
        }
        cycle++;
    }
    cf->exec();
    if(state == STATE_END) state = 0x01;
}

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

/* Status Errors */
#define STAT_ERR_CONFIG 0x01


/* The CanFix class represents our CAN-FIX connection */
CanFix *cf;
FRAM_SPI *flash;
Config cfg;
Analog analogs[11];

uint16_t status = 0x0000;
CFParameter p;
int x;
long update_rate = 500;
long next = update_rate;
volatile long counter1 = 0;
volatile long counter2 = 0;
volatile long counter3 = 0;
volatile long counter4 = 0;

void isr1(void) {
  counter1++;
}

void isr2(void) {
  counter2++;
}

void isr3(void) {
  counter3++;
}

void isr4(void) {
  counter4++;
}



void report_callback(void) {
    Serial.println("Report Callback");
}

void configure(void) {
    analogs[0].configure(2, cfg, ANALOG_RESISTANCE);
    analogs[1].configure(19, cfg, ANALOG_RESISTANCE);
    analogs[2].configure(36, cfg, ANALOG_RESISTANCE);
    analogs[3].configure(53, cfg, ANALOG_RESISTANCE);
    analogs[4].configure(70, cfg, ANALOG_RESISTANCE);
    analogs[5].configure(87, cfg, ANALOG_VOLTAGE);
    analogs[6].configure(104, cfg, ANALOG_VOLTAGE);
    analogs[7].configure(121, cfg, ANALOG_VOLTAGE);
    analogs[8].configure(138, cfg, ANALOG_VOLTAGE);
    analogs[9].configure(155, cfg, ANALOG_VOLTAGE);
    analogs[10].configure(172, cfg, ANALOG_VOLTAGE);

}

void setup() {
    uint8_t result;
    long now, last = 0;
    uint16_t dummy, key;
    
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
    flash->read(0x0000, (uint8_t *)&x, 2);

    // Writing a test configuration to the flash
    key = 2;
    dummy = 546;  cfg.writeConfig(key++, (uint8_t *)&dummy); // ID
    dummy = 0;    cfg.writeConfig(key++, (uint8_t *)&dummy); // index
    dummy = 10;    cfg.writeConfig(key++, (uint8_t *)&dummy); // Raw 0
    dummy = 0;    cfg.writeConfig(key++, (uint8_t *)&dummy); // Value 0
    dummy = 300; cfg.writeConfig(key++, (uint8_t *)&dummy); // Raw 1
    dummy = 3000; cfg.writeConfig(key++, (uint8_t *)&dummy); // Value 1
    dummy = 300; cfg.writeConfig(key++, (uint8_t *)&dummy); // Raw 2
    dummy = 3000; cfg.writeConfig(key++, (uint8_t *)&dummy); // Value 2
    dummy = 300; cfg.writeConfig(key++, (uint8_t *)&dummy); // Raw 3
    dummy = 3000; cfg.writeConfig(key++, (uint8_t *)&dummy); // Value 3
    dummy = 300; cfg.writeConfig(key++, (uint8_t *)&dummy); // Raw 4
    dummy = 3000; cfg.writeConfig(key++, (uint8_t *)&dummy); // Value 4
    dummy = 0;    cfg.writeConfig(key++, (uint8_t *)&dummy); // Min
    dummy = 3000; cfg.writeConfig(key++, (uint8_t *)&dummy); // Max
    dummy = 0;    cfg.writeConfig(key++, (uint8_t *)&dummy); // Low Warn
    dummy = 0;    cfg.writeConfig(key++, (uint8_t *)&dummy); // Low Alarm
    dummy = 3000; cfg.writeConfig(key++, (uint8_t *)&dummy); // High Warn
    dummy = 3000; cfg.writeConfig(key++, (uint8_t *)&dummy); // High Alarm
    
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
    long now, val;
    int ai;
    now = millis();
    if(now > next) {
        next += 500;
        p.type = 0x200;
        p.index = 0;
        p.fcb = 0x00;
        p.data[0] = x;
        p.data[1] = x>>8;
        p.length = 2;
        x++;
        flash->writeEnable(0x01);
        flash->write(0x0000, (uint8_t *)&x, 2);
        flash->writeEnable(0x00);
        if(x > 2600) x = 2400;
        cf->sendParam(p);

        analogs[5].read();
        p.type = analogs[5].pid;
        *(uint16_t *)p.data = analogs[5].value;
        cf->sendParam(p);
    }
    cf->exec();
}

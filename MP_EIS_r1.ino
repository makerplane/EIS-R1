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

uint16_t status = 0x0000;
CFParameter p;
int x = 2000;
long last = 0;
long update_rate = 1000;

void report_callback(void) {
    Serial.println("Report Callback");
}

void setup() {
    uint8_t result;
    long now;
    
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
    flash->read(0x0000, (uint8_t *)&x, 2);
#endif    
}


void loop() {
    long now;
    now = millis();
    if(now - last > update_rate) {
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
        last = now;
    }
    cf->exec();
}

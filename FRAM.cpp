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

#include "FRAM.h"
#include <SPI.h>


void FRAM_SPI::getDeviceID(uint8_t *manufacturerID, uint16_t *productID) {
    uint8_t x[4];

    SPI.beginTransaction(FRAM_SETTINGS);
    digitalWrite(cs_pin, LOW);
    spi->transfer(OPCODE_RDID);
    x[0] = spi->transfer(0);
    x[1] = spi->transfer(0);
    x[2] = spi->transfer(0);
    x[3] = spi->transfer(0);
    digitalWrite(cs_pin, HIGH);
    SPI.endTransaction();

    *manufacturerID = (x[0]);
    *productID = (x[2] << 8) + x[3];
}

FRAM_SPI::FRAM_SPI(uint8_t cs, uint8_t hold, uint8_t wp, SPIClass *theSPI) {
    uint8_t manufacturerID;
    uint16_t productID;

    cs_pin = cs;
    hold_pin = hold;
    wp_pin = wp;
    *spi = *theSPI;

    pinMode(cs_pin, OUTPUT);
    pinMode(hold_pin, OUTPUT);
    pinMode(wp_pin, OUTPUT);
    digitalWrite(cs_pin, HIGH);
    digitalWrite(hold_pin, HIGH);
    digitalWrite(wp_pin, HIGH);
    /*  We read the device id from the chip to make sure that we are
     *  talking to it.  Set the status to true is not.
     */
    getDeviceID(&manufacturerID, &productID);
    _status = 0x00;
    if (manufacturerID != 0x04 && manufacturerID != 0x7f) {
        _status = 0x01;
    }
    if (productID != 0x0302 && productID != 0x7f7f) {
        _status = 0x01;
    }
}

void FRAM_SPI::writeEnable(bool enable) {
    SPI.beginTransaction(FRAM_SETTINGS);
    digitalWrite(cs_pin, LOW);
    if (enable) {
        spi->transfer(OPCODE_WREN);
    } else {
        spi->transfer(OPCODE_WRDI);
    }
    digitalWrite(cs_pin, HIGH);
    SPI.endTransaction();
}

void FRAM_SPI::write(uint16_t addr, const uint8_t *values, size_t count) {
    SPI.beginTransaction(FRAM_SETTINGS);
    digitalWrite(cs_pin, LOW);
    spi->transfer(OPCODE_WRITE);
    spi->transfer((uint8_t)(addr >> 8));
    spi->transfer((uint8_t)(addr & 0xFF));
    for (size_t i = 0; i < count; i++) {
        spi->transfer(values[i]);
    }
    /* CS on the rising edge commits the WRITE */
    digitalWrite(cs_pin, HIGH);
    SPI.endTransaction();
}

void FRAM_SPI::read(uint16_t addr, uint8_t *values, size_t count) {
    SPI.beginTransaction(FRAM_SETTINGS);
    digitalWrite(cs_pin, LOW);
    spi->transfer(OPCODE_READ);
    spi->transfer((uint8_t)(addr >> 8));
    spi->transfer((uint8_t)(addr & 0xFF));
    for (size_t i = 0; i < count; i++) {
        uint8_t x = spi->transfer(0);
        values[i] = x;
    }
    digitalWrite(cs_pin, HIGH);
    SPI.endTransaction();
}

uint8_t FRAM_SPI::getStatus(void) {
    return _status;
}

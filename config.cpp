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

/*
 *  This is the configuration system of the device.  Configuration can be
 *  stored in RAM, FRAM or EEPROM.  The Config class just needs read/write
 *  functions to be given for the appropriate storage.  There is a map
 *  that represents the index and size of each configuration item but this
 *  class is agnostic as to what the configuration items represent.  The
 *  validity and type of data is handled elsewhere.
 */

#include "config.h"

/*  This represents a map of where we find each configuration value.  The
 *  index to the array is the canfix configuration key.  The value itself
 *  encodes the memory address offset into the higher 4 bits and the length into
 *  the lower 2 bits.
 */


//const PROGMEM uint16_t configMap[] = { (0 << 2) + 1, // Engine Number
const uint16_t configMap[] =         { (0 << 3) + 1, // Engine Number
                                       (1 << 3) + 1, // Cylinder Count
                                       (2 << 3) + 2, // ID
                                       (4 << 3) + 2, // Index
                                       (6 << 3) + 1, // Function
                                       (8 << 3) + 2, // Raw 1
                                       (10 << 3) + 2, // Raw 2
                                       (12 << 3) + 2, // Raw 3
                                       (14 << 3) + 2, // Raw 4
                                       (16 << 3) + 2, // Raw 5
                                       (18 << 3) + 4, // A
                                       (22 << 3) + 4, // B
                                       (26 << 3) + 4, // C
                                       (30 << 3) + 4, // D
                                       (34 << 3) + 4, // E
                                       (38 << 3) + 4, // Min
                                       (42 << 3) + 4, // Max
                                       (46 << 3) + 4, // Low Warn
                                       (50 << 3) + 4, // Low Alarm
                                       (54 << 3) + 4, // High Warn
                                       (58 << 3) + 4, // High Alarm
                                     };
/*
void Config::setWriteFunction(void (*func)(uint16_t addr, uint8_t *values, size_t count)) {
    writefunc = func;
}

void Config::setReadFunction(void (*func)(uint16_t addr, uint8_t *values, size_t count)) {
    readfunc = func;
}
*/

#define CFG_START_ADDR 128


void Config::setFlash(FRAM_SPI *f) {
    flash = f;
}

void Config::getAddr(uint16_t key) {
    addr = CFG_START_ADDR + (configMap[key] >> 3);
    len  = configMap[key] & 0x0007;

    // if(key < 2) {
    //     addr = CFG_START_ADDR + key;
    //     len = 1;
    // } else {
    //     index = (key - 2) / 21;
    //     offset = (key - 2) % 21;
    //     addr = (key-2) * 2 + CFG_START_ADDR + 2;
    //     len = 2;
    // }

}

uint8_t Config::readConfig(uint16_t key, uint8_t *data) {
    getAddr(key);

    flash->read(addr, data, len);

    return len;
}

void Config::writeConfig(uint16_t key, void *data) {
    getAddr(key);
    // Serial.print(key);
    // Serial.print(" ");
    // Serial.print(addr);
    // Serial.print(" ");
    // Serial.println(len);

    flash->writeEnable(0x01);
    flash->write(addr, data, len);
    flash->writeEnable(0x00);
}

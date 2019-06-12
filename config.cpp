/*  dCopyright (c) 2019 Phil Birkelbach
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
 *  encodes the memory address into the higher 4 bits and the length into
 *  the lower 2 bits.
 */

/*
const PROGMEM uint16_t configMap[] = { (128 << 2) + 1, // Engine Number
                                       (129 << 2) + 1, // Cylinder Count
                                       (130 << 2) + 2, // Res Input 1 CF ID
                                       (132 << 2) + 2, // Res Input 1 CF Index
                                       (134 << 2) + 2, // Res Input 1 Raw 1
                                       (136 << 2) + 2, // Res Input 1 Val 1
                                       (138 << 2) + 2, // Res Input 1 Raw 2
                                       (140 << 2) + 2, // Res Input 1 Val 2
                                       (142 << 2) + 2, // Res Input 1 Raw 3
                                       (144 << 2) + 2, // Res Input 1 Val 3
                                       (146 << 2) + 2, // Res Input 1 Raw 4
                                       (148 << 2) + 2, // Res Input 1 Val 4
                                       (150 << 2) + 2, // Res Input 1 Raw 5
                                       (152 << 2) + 2, // Res Input 1 Val 5
                                       
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
    if(key < 2) {
        addr = CFG_START_ADDR + key;
        len = 1;
    } else {
        addr = (key-2) * 2 + CFG_START_ADDR + 2;
        len = 2;
    }
    
}

void Config::readConfig(uint16_t key, uint8_t *data) {
    getAddr(key);
    //memcpy_P(&mapEntry, &configMap[key], 2);
    //addr = (mapEntry & 0xFFFC) >> 2;
    //len = mapEntry & 0x0003;
    flash->read(addr, data, len);
    Serial.print("Reading ");
    Serial.print(key);
    Serial.print(" Address ");
    Serial.println(addr);
}

void Config::writeConfig(uint16_t key, uint8_t *data) {
    getAddr(key);
    //memcpy_P(&mapEntry, &configMap[key], 2);
    //addr = (mapEntry & 0xFFFC) >> 2;
    //len = mapEntry & 0x0003;
    flash->writeEnable(0x01);
    flash->write(addr, data, len);
    flash->writeEnable(0x00);
}

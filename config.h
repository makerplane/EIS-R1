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

#ifndef __CONFIG_H
#define __CONFIG_H

//#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <avr/pgmspace.h>
#include "FRAM.h"

// union Config_t {
//     uint8_t uShort;
//     uint16_t uInt;
//     int16_t sInt;
// };


class Config {
    public:
        void setFlash(FRAM_SPI *f);
        uint8_t readConfig(uint16_t key, uint8_t *data);
        void writeConfig(uint16_t key, void *data);
    private:
        FRAM_SPI *flash;
        size_t len;
        uint16_t addr;
        void getAddr(uint16_t key);

};

#endif

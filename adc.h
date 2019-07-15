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

/* This program is the MakerPlane Engine Information System firmware.
   This file contains the code for reading the ADC1118 information.  This
   includes the thermocouples, the unit temperature and the system voltage
*/

#ifndef __ADC_H
#define __ADC_H

#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>

#define ADC_CS       9
#define ADC_MUX0    11
#define ADC_MUX1    12
#define ADC_MUX2    13

#define ADC_TYPE_CHT  0x01
#define ADC_TYPE_EGT  0x02
#define ADC_TYPE_VOLT  0x03
#define ADC_TYPE_TEMP  0x04


class ADC1118 {
    public:
        uint16_t voltage;  // Voltage * 10
        uint16_t int_temperture;
        uint16_t chts[6];
        uint16_t egts[6];
        void start_sample(uint8_t select, uint8_t ts);
        void read_sample(void);

    private:
        uint8_t mux_select;
        uint8_t sample_type;

};

#endif

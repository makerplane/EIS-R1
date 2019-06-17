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
   This class represents one of the 10 bit analog inputs.
*/

#ifndef __ANALOGS_H
#define __ANALOGS_H

#define ANALOG_RESISTANCE 1
#define ANALOG_VOLTAGE 2


#include <stdint.h>
#include "config.h"
#include "canfix.h"

class Analog {
    public:
        byte flags, type;
        int input_pin;
        uint16_t pid, index, value, rawValue;
        uint16_t minimum, maximum, lowWarn, highWarn, lowAlarm, highAlarm;
        void configure(uint16_t keystart, Config cfg, byte type);
        //void setPin(uint8_t pin);
        void read(void);

    private:
        uint16_t raw[5], scaled[5];

};

#endif

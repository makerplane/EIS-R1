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

#define FUNC_LINEAR 0
#define FUNC_5_STEP 1
#define FUNC_POLY   2
#define FUNC_EXP    3
#define FUNC_STEIN  4
#define FUNC_BETA   5


#include <stdint.h>
#include "config.h"
#include "canfix.h"

#define AFL_ENG_INC 0x01

struct ParamDefinition {
    unsigned int pid;
    unsigned char mult;
    unsigned char flags;
    float min;
    float max;
};


class Analog {
    public:
        byte quality, type, functype;
        int input_pin;
        uint16_t pid, index, value, rawValue;
        float multiplier, min, max;
        // Aux Data
        float auxmin, auxmax, lowWarn, highWarn, lowAlarm, highAlarm;
        void configure(uint16_t keystart, Config *cfg, byte type);
        void read(void);

    private:
        float scaled[5], raw[5];
        struct ParamDefinition *findParamDef(uint16_t pid);
};

#endif

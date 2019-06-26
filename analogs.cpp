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

#include "analogs.h"


//void Analog::setPin(uint8_t pin) {
//    input_pin = pin;
//}

void Analog::configure(uint16_t keystart, Config *cfg, byte t) {
    byte n;
    unsigned int key = keystart;
    uint16_t result;
    type = t;
    cfg->readConfig(key++, (uint8_t *)&result);
    pid = result >> 4;
    index = result & 0x000F;
    cfg->readConfig(key++, (uint8_t *)&functype);

    for(n=0;n<5;n++) {
        cfg->readConfig(key++, (uint8_t *)&result);
        if(type == ANALOG_RESISTANCE) {
            raw[n] = 1024l*result/(300l+result);
        } else {
            raw[n] = 1023l*result / 5000l;
        }
    }
    for(n=0;n<5;n++) {
        cfg->readConfig(key++, (uint8_t *)(scaled +n));
    }
    cfg->readConfig(key++, (uint8_t *)&minimum);
    cfg->readConfig(key++, (uint8_t *)&maximum);
    cfg->readConfig(key++, (uint8_t *)&lowWarn);
    cfg->readConfig(key++, (uint8_t *)&lowAlarm);
    cfg->readConfig(key++, (uint8_t *)&highWarn);
    cfg->readConfig(key++, (uint8_t *)&highAlarm);
}

void Analog::read(void) {
    uint16_t ai;
    float x;
    ai = analogRead(input_pin);
    if(type == ANALOG_RESISTANCE) {
      rawValue = 300ul*ai/(1024ul-ai);
    } else {
      rawValue = (5000ul*ai) / 1023;
    }
    if(functype == FUNC_LINEAR) {
       x = scaled[0] + (ai - raw[0])*(scaled[1] - scaled[0])/(raw[1] - raw[0]);
       value = x * 10;
    }
    // for(n=0;n<4;n++) {
    //     if(ai >= raw[n] && ai <= raw[n+1]) {
    //       x = n;
    //       break;
    //     }
    // }
    // if(x==5) {  //This indicates out of range of the configuration
    //     flags = FCB_FAIL;
    //     value = 0x00;
    //     return;
    // }
    // value = scaled[x] + ((long long)ai - raw[x])*(scaled[x+1] - scaled[x])/(raw[x+1] - raw[x]);

    //value = 0 + ( (ai - 0)*(3000l - 0))/(1023l - 0);
    //value = (10000ul * ai) / 1023;
}

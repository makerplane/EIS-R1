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

struct ParamDefinition paramDefinition[] =
         {{0x21C,100,AFL_ENG_INC | AFL_LOW_BYP,0,100}, // Fuel Pressure
          {0x21E,100,AFL_ENG_INC,0,60}, // Manifold Pressure
          {0x220,100,AFL_ENG_INC | AFL_LOW_BYP,0,100}, // Oil Pressure
          {0x222,10,AFL_ENG_INC | AFL_LOW_BYP,0,300}, // Oil Temperature
          {0x224,10,AFL_ENG_INC | AFL_LOW_BYP,0,300}, // Coolant Temperature
          {0x226,100,0,0,300}, // Fuel Quantity
          {0x50C,10,AFL_ENG_INC | AFL_LOW_BYP,0,300}, // Carburetor Temperature
          {0x583,10,0,0,300}, // Cabin Temperature
          {0x406,100,0,-300,300}, // Outside Air Temperature
          {0x50E,10,0,0,36}, // Electrical Bus Voltage
          {0x512,10,0,-200,200}, // Electrical Bus Current
          {0x516,10,AFL_ENG_INC,0,36}, // Alternator Voltage
          {0x51A,10,AFL_ENG_INC,0,200}, // Alternator Current
          {0x390,100,0,0,100}, // Generic Analog
          {0x391,100,0,0,100}, // Generic Analog
          {0x392,100,0,0,100}, // Generic Analog
          {0x393,100,0,0,100}, // Generic Analog
          {0,0,0,0,0}            // The End
};


struct ParamDefinition *Analog::findParamDef(uint16_t pid) {
    byte n;
    for(n=0;n<255;n++) {
        if(paramDefinition[n].pid == pid) {
            return &paramDefinition[n];
        } else if(paramDefinition[n].pid == 0) {
            return 0x00;
        }
    }
    return 0x00;
}

void Analog::configure(uint16_t keystart, Config *cfg, byte t) {
    byte n;
    unsigned int key = keystart;
    uint16_t result;
    type = t;
    struct ParamDefinition *pd;

    cfg->readConfig(key++, (uint8_t *)&result);
    pid = result >> 4;
    index = result & 0x000F;
    pd = findParamDef(pid);
    if(pd) {
        // TODO deal with engine increment and low bypass flags
        multiplier = pd->mult;
        min = pd->min;
        max = pd->max;
    } else {
        pid = 0; // This will indicate a bad pid
    }
    cfg->readConfig(key++, (uint8_t *)&functype);

    for(n=0;n<5;n++) {
        cfg->readConfig(key++, (uint8_t *)&result);
        if(type == ANALOG_RESISTANCE) {
            raw[n] = result;
        } else {
            raw[n] = result / 1000;
        }
    }
    for(n=0;n<5;n++) {
        cfg->readConfig(key++, (uint8_t *)(scaled + n));
    }
    cfg->readConfig(key++, (uint8_t *)&auxmin);
    cfg->readConfig(key++, (uint8_t *)&auxmax);
    cfg->readConfig(key++, (uint8_t *)&lowWarn);
    cfg->readConfig(key++, (uint8_t *)&lowAlarm);
    cfg->readConfig(key++, (uint8_t *)&highWarn);
    cfg->readConfig(key++, (uint8_t *)&highAlarm);
}

void Analog::read(void) {
    uint16_t ai;
    float x=0, y;
    ai = analogRead(input_pin);
    if(type == ANALOG_RESISTANCE) {
        rawValue = 300ul*ai/(1024ul-ai); // For use in Status messages.
        x = rawValue;
    } else {
        rawValue = (5000ul*ai) / 1023;
        x = rawValue / 1000.0;
    }
    if(functype == FUNC_LINEAR) {
        y = scaled[0] + (x - raw[0])*(scaled[1] - scaled[0])/(raw[1] - raw[0]);
    } else {
        y = 0;
    }
    // TODO Deal with the rest of the functions
    // Serial.print(scaled[0]);
    // Serial.print(" ");
    // Serial.print(scaled[1]);
    // Serial.print(" ");
    // Serial.print(raw[0]);
    // Serial.print(" ");
    // Serial.print(raw[1]);
    // Serial.print(" ");
    // Serial.print(y);
    // Serial.println(" ");

    if(y > max) y = max;
    if(y < min) y = min;
    value = y * multiplier;
    // TODO Deal with negative numbers

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

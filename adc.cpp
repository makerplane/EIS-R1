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

*/

#include "adc.h"

/* Starts the ADC sample.  It's up to the calling system to make sure that the
   sample time has expired.  Select is the setting to put on the TC Mux chip
   which would determine which cylinder we are looking for, the type is the
   ADC_TYPE_?? for CHT, EGT or the Voltage */
void ADC1118::start_sample(uint8_t select, uint8_t ts)
{
    uint8_t cfgMSB;
    uint8_t cfgLSB;

    mux_select = select;
    sample_type = ts;
    // Set the mux chip to the selected thermocouple
    digitalWrite(ADC_MUX0, (mux_select & 0x01) ? HIGH : LOW);
    digitalWrite(ADC_MUX1, (mux_select & 0x02) ? HIGH : LOW);
    digitalWrite(ADC_MUX2, (mux_select & 0x04) ? HIGH : LOW);

    cfgLSB = 0x8B;
    cfgMSB = 0x00;
    if(sample_type == ADC_TYPE_CHT) {
        cfgMSB = 0xC1;
    } else  if(sample_type == ADC_TYPE_EGT) {
        cfgMSB = 0xD1;
    } else  if(sample_type == ADC_TYPE_VOLT) {
        cfgMSB = 0xE1;
    } else  if(sample_type == ADC_TYPE_TEMP) {
        cfgLSB = 0x9B;
        cfgMSB = 0xF1;
    }
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
    digitalWrite(ADC_CS, LOW);
    SPI.transfer(cfgMSB);
    SPI.transfer(cfgLSB);
    digitalWrite(ADC_CS, HIGH);
    SPI.endTransaction();
}

inline float voltage_convert(uint16_t data, float fsr)
{
    float adc_result;

    if(data>=0x8000){
        data=((~data)+1); //Applying binary twos complement format
        adc_result=((float)(data*fsr/32768)*-1);
    } else {
        adc_result=(float)(data*fsr/32768);
    }
    return adc_result;
}

/* This function reads the sample and puts it in the right place */
void ADC1118::read_sample(void)
{
    float v;
    uint8_t dataMSB;
    uint8_t dataLSB;
    uint16_t data;

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
    digitalWrite(ADC_CS, LOW);
    dataMSB = SPI.transfer(0xFF);
    dataLSB = SPI.transfer(0xFF);
    digitalWrite(ADC_CS, HIGH);
    SPI.endTransaction();
    data = (dataMSB << 8) | (dataLSB);

    if(sample_type == ADC_TYPE_CHT) {
        v = voltage_convert(data, 6.144);
        chts[mux_select] = (uint16_t)((v-1.24)/0.005 * 10);
    } else  if(sample_type == ADC_TYPE_EGT) {
        v = voltage_convert(data, 6.144);
        egts[mux_select] = (uint16_t)(v/0.005 * 10);
    } else  if(sample_type == ADC_TYPE_VOLT) {
        v = voltage_convert(data, 6.144);
        voltage = (uint16_t)(v * 10);
    } else  if(sample_type == ADC_TYPE_TEMP) {
        data = ((dataMSB << 8) | (dataLSB))>>2;
        if((data<<2) >= 0x8000){
    		data=((~data)>>2)+1; //Converting to right-justified and applying binary twos complement format
            v = (float)(data*0.03125*-1);
        }
        v = (float)data*0.03125;
        int_temperture = (uint16_t)(v * 10);
    }

}

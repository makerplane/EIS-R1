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

#ifndef __FRAM_H
#define __FRAM_H

#include <SPI.h>

 /* Flash Opcodes */
#define OPCODE_WREN 0b0110     /* Write Enable Latch */
#define OPCODE_WRDI 0b0100     /* Reset Write Enable Latch */
#define OPCODE_RDSR 0b0101     /* Read Status Register */
#define OPCODE_WRSR 0b0001     /* Write Status Register */
#define OPCODE_READ 0b0011     /* Read Memory */
#define OPCODE_WRITE 0b0010    /* Write Memory */
#define OPCODE_RDID 0b10011111 /* Read Device ID */

class FRAM_SPI {
    public:
        FRAM_SPI(uint8_t cs, uint8_t hold, uint8_t wp, SPIClass *theSPI = &SPI);
        void writeEnable(bool enable);
        void write(uint16_t addr, const uint8_t *values, size_t count);
        void read(uint16_t addr, uint8_t *values, size_t count);
        uint8_t getStatus(void);
    private:
        uint8_t cs_pin, hold_pin, wp_pin, _status;
        SPIClass *spi;

        void getDeviceID(uint8_t *manufacturerID, uint16_t *productID);
};

#endif

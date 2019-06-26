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
 *  encodes the memory address offset into the higher 13 bits and the length into
 *  the lower 3 bits. A zero would mean a bad config key.
 */

//const uint16_t configMap[] = {
const uint16_t configMap[] PROGMEM = {
  (128 << 3) + 1, // Engine Number
  (129 << 3) + 1, // Cylinder Count
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (148 << 3) + 2, // Analog Input 1 Selection
  (150 << 3) + 1, // Analog Input 1 Function Select
  (151 << 3) + 2, // Analog Input 1 Raw Input #1
  (153 << 3) + 2, // Analog Input 1 Raw Input #2
  (155 << 3) + 2, // Analog Input 1 Raw Input #3
  (157 << 3) + 2, // Analog Input 1 Raw Input #4
  (159 << 3) + 2, // Analog Input 1 Raw Input #5
  (161 << 3) + 4, // Analog Input 1 Value A
  (165 << 3) + 4, // Analog Input 1 Value B
  (169 << 3) + 4, // Analog Input 1 Value C
  (173 << 3) + 4, // Analog Input 1 Value D
  (177 << 3) + 4, // Analog Input 1 Value E
  (181 << 3) + 4, // Analog Input 1 Min
  (185 << 3) + 4, // Analog Input 1 Min
  (189 << 3) + 4, // Analog Input 1 Low Warning
  (193 << 3) + 4, // Analog Input 1 Low Alarm
  (197 << 3) + 4, // Analog Input 1 High Warning
  (201 << 3) + 4, // Analog Input 1 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (221 << 3) + 2, // Analog Input 2 Selection
  (223 << 3) + 1, // Analog Input 2 Function Select
  (224 << 3) + 2, // Analog Input 2 Raw Input #1
  (226 << 3) + 2, // Analog Input 2 Raw Input #2
  (228 << 3) + 2, // Analog Input 2 Raw Input #3
  (230 << 3) + 2, // Analog Input 2 Raw Input #4
  (232 << 3) + 2, // Analog Input 2 Raw Input #5
  (234 << 3) + 4, // Analog Input 2 Value A
  (238 << 3) + 4, // Analog Input 2 Value B
  (242 << 3) + 4, // Analog Input 2 Value C
  (246 << 3) + 4, // Analog Input 2 Value D
  (250 << 3) + 4, // Analog Input 2 Value E
  (254 << 3) + 4, // Analog Input 2 Min
  (258 << 3) + 4, // Analog Input 2 Min
  (262 << 3) + 4, // Analog Input 2 Low Warning
  (266 << 3) + 4, // Analog Input 2 Low Alarm
  (270 << 3) + 4, // Analog Input 2 High Warning
  (274 << 3) + 4, // Analog Input 2 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (294 << 3) + 2, // Analog Input 3 Selection
  (296 << 3) + 1, // Analog Input 3 Function Select
  (297 << 3) + 2, // Analog Input 3 Raw Input #1
  (299 << 3) + 2, // Analog Input 3 Raw Input #2
  (301 << 3) + 2, // Analog Input 3 Raw Input #3
  (303 << 3) + 2, // Analog Input 3 Raw Input #4
  (305 << 3) + 2, // Analog Input 3 Raw Input #5
  (307 << 3) + 4, // Analog Input 3 Value A
  (311 << 3) + 4, // Analog Input 3 Value B
  (315 << 3) + 4, // Analog Input 3 Value C
  (319 << 3) + 4, // Analog Input 3 Value D
  (323 << 3) + 4, // Analog Input 3 Value E
  (327 << 3) + 4, // Analog Input 3 Min
  (331 << 3) + 4, // Analog Input 3 Min
  (335 << 3) + 4, // Analog Input 3 Low Warning
  (339 << 3) + 4, // Analog Input 3 Low Alarm
  (343 << 3) + 4, // Analog Input 3 High Warning
  (347 << 3) + 4, // Analog Input 3 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (367 << 3) + 2, // Analog Input 4 Selection
  (369 << 3) + 1, // Analog Input 4 Function Select
  (370 << 3) + 2, // Analog Input 4 Raw Input #1
  (372 << 3) + 2, // Analog Input 4 Raw Input #2
  (374 << 3) + 2, // Analog Input 4 Raw Input #3
  (376 << 3) + 2, // Analog Input 4 Raw Input #4
  (378 << 3) + 2, // Analog Input 4 Raw Input #5
  (380 << 3) + 4, // Analog Input 4 Value A
  (384 << 3) + 4, // Analog Input 4 Value B
  (388 << 3) + 4, // Analog Input 4 Value C
  (392 << 3) + 4, // Analog Input 4 Value D
  (396 << 3) + 4, // Analog Input 4 Value E
  (400 << 3) + 4, // Analog Input 4 Min
  (404 << 3) + 4, // Analog Input 4 Min
  (408 << 3) + 4, // Analog Input 4 Low Warning
  (412 << 3) + 4, // Analog Input 4 Low Alarm
  (416 << 3) + 4, // Analog Input 4 High Warning
  (420 << 3) + 4, // Analog Input 4 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (440 << 3) + 2, // Analog Input 5 Selection
  (442 << 3) + 1, // Analog Input 5 Function Select
  (443 << 3) + 2, // Analog Input 5 Raw Input #1
  (445 << 3) + 2, // Analog Input 5 Raw Input #2
  (447 << 3) + 2, // Analog Input 5 Raw Input #3
  (449 << 3) + 2, // Analog Input 5 Raw Input #4
  (451 << 3) + 2, // Analog Input 5 Raw Input #5
  (453 << 3) + 4, // Analog Input 5 Value A
  (457 << 3) + 4, // Analog Input 5 Value B
  (461 << 3) + 4, // Analog Input 5 Value C
  (465 << 3) + 4, // Analog Input 5 Value D
  (469 << 3) + 4, // Analog Input 5 Value E
  (473 << 3) + 4, // Analog Input 5 Min
  (477 << 3) + 4, // Analog Input 5 Min
  (481 << 3) + 4, // Analog Input 5 Low Warning
  (485 << 3) + 4, // Analog Input 5 Low Alarm
  (489 << 3) + 4, // Analog Input 5 High Warning
  (493 << 3) + 4, // Analog Input 5 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (513 << 3) + 2, // Analog Input 6 Selection
  (515 << 3) + 1, // Analog Input 6 Function Select
  (516 << 3) + 2, // Analog Input 6 Raw Input #1
  (518 << 3) + 2, // Analog Input 6 Raw Input #2
  (520 << 3) + 2, // Analog Input 6 Raw Input #3
  (522 << 3) + 2, // Analog Input 6 Raw Input #4
  (524 << 3) + 2, // Analog Input 6 Raw Input #5
  (526 << 3) + 4, // Analog Input 6 Value A
  (530 << 3) + 4, // Analog Input 6 Value B
  (534 << 3) + 4, // Analog Input 6 Value C
  (538 << 3) + 4, // Analog Input 6 Value D
  (542 << 3) + 4, // Analog Input 6 Value E
  (546 << 3) + 4, // Analog Input 6 Min
  (550 << 3) + 4, // Analog Input 6 Min
  (554 << 3) + 4, // Analog Input 6 Low Warning
  (558 << 3) + 4, // Analog Input 6 Low Alarm
  (562 << 3) + 4, // Analog Input 6 High Warning
  (566 << 3) + 4, // Analog Input 6 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (586 << 3) + 2, // Analog Input 7 Selection
  (588 << 3) + 1, // Analog Input 7 Function Select
  (589 << 3) + 2, // Analog Input 7 Raw Input #1
  (591 << 3) + 2, // Analog Input 7 Raw Input #2
  (593 << 3) + 2, // Analog Input 7 Raw Input #3
  (595 << 3) + 2, // Analog Input 7 Raw Input #4
  (597 << 3) + 2, // Analog Input 7 Raw Input #5
  (599 << 3) + 4, // Analog Input 7 Value A
  (603 << 3) + 4, // Analog Input 7 Value B
  (607 << 3) + 4, // Analog Input 7 Value C
  (611 << 3) + 4, // Analog Input 7 Value D
  (615 << 3) + 4, // Analog Input 7 Value E
  (619 << 3) + 4, // Analog Input 7 Min
  (623 << 3) + 4, // Analog Input 7 Min
  (627 << 3) + 4, // Analog Input 7 Low Warning
  (631 << 3) + 4, // Analog Input 7 Low Alarm
  (635 << 3) + 4, // Analog Input 7 High Warning
  (639 << 3) + 4, // Analog Input 7 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (659 << 3) + 2, // Analog Input 8 Selection
  (661 << 3) + 1, // Analog Input 8 Function Select
  (662 << 3) + 2, // Analog Input 8 Raw Input #1
  (664 << 3) + 2, // Analog Input 8 Raw Input #2
  (666 << 3) + 2, // Analog Input 8 Raw Input #3
  (668 << 3) + 2, // Analog Input 8 Raw Input #4
  (670 << 3) + 2, // Analog Input 8 Raw Input #5
  (672 << 3) + 4, // Analog Input 8 Value A
  (676 << 3) + 4, // Analog Input 8 Value B
  (680 << 3) + 4, // Analog Input 8 Value C
  (684 << 3) + 4, // Analog Input 8 Value D
  (688 << 3) + 4, // Analog Input 8 Value E
  (692 << 3) + 4, // Analog Input 8 Min
  (696 << 3) + 4, // Analog Input 8 Min
  (700 << 3) + 4, // Analog Input 8 Low Warning
  (704 << 3) + 4, // Analog Input 8 Low Alarm
  (708 << 3) + 4, // Analog Input 8 High Warning
  (712 << 3) + 4, // Analog Input 8 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (732 << 3) + 2, // Analog Input 9 Selection
  (734 << 3) + 1, // Analog Input 9 Function Select
  (735 << 3) + 2, // Analog Input 9 Raw Input #1
  (737 << 3) + 2, // Analog Input 9 Raw Input #2
  (739 << 3) + 2, // Analog Input 9 Raw Input #3
  (741 << 3) + 2, // Analog Input 9 Raw Input #4
  (743 << 3) + 2, // Analog Input 9 Raw Input #5
  (745 << 3) + 4, // Analog Input 9 Value A
  (749 << 3) + 4, // Analog Input 9 Value B
  (753 << 3) + 4, // Analog Input 9 Value C
  (757 << 3) + 4, // Analog Input 9 Value D
  (761 << 3) + 4, // Analog Input 9 Value E
  (765 << 3) + 4, // Analog Input 9 Min
  (769 << 3) + 4, // Analog Input 9 Min
  (773 << 3) + 4, // Analog Input 9 Low Warning
  (777 << 3) + 4, // Analog Input 9 Low Alarm
  (781 << 3) + 4, // Analog Input 9 High Warning
  (785 << 3) + 4, // Analog Input 9 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (805 << 3) + 2, // Analog Input 10 Selection
  (807 << 3) + 1, // Analog Input 10 Function Select
  (808 << 3) + 2, // Analog Input 10 Raw Input #1
  (810 << 3) + 2, // Analog Input 10 Raw Input #2
  (812 << 3) + 2, // Analog Input 10 Raw Input #3
  (814 << 3) + 2, // Analog Input 10 Raw Input #4
  (816 << 3) + 2, // Analog Input 10 Raw Input #5
  (818 << 3) + 4, // Analog Input 10 Value A
  (822 << 3) + 4, // Analog Input 10 Value B
  (826 << 3) + 4, // Analog Input 10 Value C
  (830 << 3) + 4, // Analog Input 10 Value D
  (834 << 3) + 4, // Analog Input 10 Value E
  (838 << 3) + 4, // Analog Input 10 Min
  (842 << 3) + 4, // Analog Input 10 Min
  (846 << 3) + 4, // Analog Input 10 Low Warning
  (850 << 3) + 4, // Analog Input 10 Low Alarm
  (854 << 3) + 4, // Analog Input 10 High Warning
  (858 << 3) + 4, // Analog Input 10 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (878 << 3) + 2, // Analog Input 11 Selection
  (880 << 3) + 1, // Analog Input 11 Function Select
  (881 << 3) + 2, // Analog Input 11 Raw Input #1
  (883 << 3) + 2, // Analog Input 11 Raw Input #2
  (885 << 3) + 2, // Analog Input 11 Raw Input #3
  (887 << 3) + 2, // Analog Input 11 Raw Input #4
  (889 << 3) + 2, // Analog Input 11 Raw Input #5
  (891 << 3) + 4, // Analog Input 11 Value A
  (895 << 3) + 4, // Analog Input 11 Value B
  (899 << 3) + 4, // Analog Input 11 Value C
  (903 << 3) + 4, // Analog Input 11 Value D
  (907 << 3) + 4, // Analog Input 11 Value E
  (911 << 3) + 4, // Analog Input 11 Min
  (915 << 3) + 4, // Analog Input 11 Min
  (919 << 3) + 4, // Analog Input 11 Low Warning
  (923 << 3) + 4, // Analog Input 11 Low Alarm
  (927 << 3) + 4, // Analog Input 11 High Warning
  (931 << 3) + 4, // Analog Input 11 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (951 << 3) + 2, // Tach Input 1 Selection
  (953 << 3) + 2, // Tach Input 1 Pulses Per Revolution
  (955 << 3) + 2, // Tach Input 1 Low Warning
  (957 << 3) + 2, // Tach Input 1 Low Alarm
  (959 << 3) + 2, // Tach Input 1 High Warning
  (961 << 3) + 2, // Tach Input 1 High Alarm
  (963 << 3) + 2, // Tach Input 1 Low Restriction 1
  (965 << 3) + 2, // Tach Input 1 High Restriction 1
  (967 << 3) + 2, // Tach Input 1 Low Restriction 2
  (969 << 3) + 2, // Tach Input 1 High Restriction 2
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (989 << 3) + 2, // Tach Input 2 Selection
  (991 << 3) + 2, // Tach Input 2 Pulses Per Revolution
  (993 << 3) + 2, // Tach Input 2 Low Warning
  (995 << 3) + 2, // Tach Input 2 Low Alarm
  (997 << 3) + 2, // Tach Input 2 High Warning
  (999 << 3) + 2, // Tach Input 2 High Alarm
  (1001 << 3) + 2, // Tach Input 2 Low Restriction 1
  (1003 << 3) + 2, // Tach Input 2 High Restriction 1
  (1005 << 3) + 2, // Tach Input 2 Low Restriction 2
  (1007 << 3) + 2, // Tach Input 2 High Restriction 2
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (1027 << 3) + 2, // Flow Input 1 Selection
  (1029 << 3) + 2, // Flow Input 1 Pulses Per Gallon
  (1031 << 3) + 2, // Flow Input 1 Min
  (1033 << 3) + 2, // Flow Input 1 Max
  (1035 << 3) + 2, // Flow Input 1 Low Warning
  (1037 << 3) + 2, // Flow Input 1 Low Alarm
  (1039 << 3) + 2, // Flow Input 1 High Warning
  (1041 << 3) + 2, // Flow Input 1 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (1061 << 3) + 2, // Flow Input 2 Selection
  (1063 << 3) + 2, // Flow Input 2 Pulses Per Gallon
  (1065 << 3) + 2, // Flow Input 2 Min
  (1067 << 3) + 2, // Flow Input 2 Max
  (1069 << 3) + 2, // Flow Input 2 Low Warning
  (1071 << 3) + 2, // Flow Input 2 Low Alarm
  (1073 << 3) + 2, // Flow Input 2 High Warning
  (1075 << 3) + 2, // Flow Input 2 High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (1095 << 3) + 2, // CHT Min
  (1097 << 3) + 2, // CHT Max
  (1099 << 3) + 2, // CHT Low Warning
  (1101 << 3) + 2, // CHT Low Alarm
  (1103 << 3) + 2, // CHT High Warning
  (1105 << 3) + 2, // CHT High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (1125 << 3) + 2, // EGT Min
  (1127 << 3) + 2, // EGT Max
  (1129 << 3) + 2, // EGT Low Warning
  (1131 << 3) + 2, // EGT Low Alarm
  (1133 << 3) + 2, // EGT High Warning
  (1135 << 3) + 2, // EGT High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (1155 << 3) + 2, // CHT Rate Of Change Min
  (1157 << 3) + 2, // CHT Rate Of Change Max
  (1159 << 3) + 2, // CHT Rate Of Change High Warning
  (1161 << 3) + 2, // CHT Rate Of Change High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (1181 << 3) + 2, // CHT Deviation Min
  (1183 << 3) + 2, // CHT Deviation Max
  (1185 << 3) + 2, // CHT Deviation High Warning
  (1187 << 3) + 2, // CHT Deviation High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (1207 << 3) + 2, // EGT Rate Of Change Min
  (1209 << 3) + 2, // EGT Rate Of Change Max
  (1211 << 3) + 2, // EGT Rate Of Change High Warning
  (1213 << 3) + 2, // EGT Rate Of Change High Alarm
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (0 << 3) + 0, //
  (1233 << 3) + 2, // EGT Deviation Min
  (1235 << 3) + 2, // EGT Deviation Max
  (1237 << 3) + 2, // EGT Deviation High Warning
  (1239 << 3) + 2, // EGT Deviation High Alarm
};


//#define CFG_START_ADDR 128


void Config::setFlash(FRAM_SPI *f) {
    flash = f;
}

/* This function reads the configuration map from the
   program memory array above and writes the private
   addr and len values.  If a key is invalid it writes
   0x00 to len. */
void Config::getAddr(uint16_t key) {
    unsigned int result;
    if(key > 351) { // Key out of bounds
        len = 0x00;
        return;
    }
    result = pgm_read_word_far(configMap + key);
    addr = result >> 3;
    len = result & 0x0007;
}

uint8_t Config::readConfig(uint16_t key, uint8_t *data) {
    getAddr(key);
    if(len == 0) return 0;

    flash->read(addr, (uint8_t *)data, len);

    return len;
}

uint8_t Config::writeConfig(uint16_t key, void *data) {
    getAddr(key);
    if(len == 0) return 0;

    flash->writeEnable(0x01);
    flash->write(addr, (uint8_t *)data, len);
    flash->writeEnable(0x00);
    return len;
}

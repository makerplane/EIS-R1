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
    (128 << 3) + 1,  // Engine Number
    (129 << 3) + 1,  // Cylinder Count
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (148 << 3) + 2,  // Analog Input 1 Selection
    (150 << 3) + 1,  // Analog Input 1 Function Select
    (151 << 3) + 2,  // Analog Input 1 Raw Input #1
    (153 << 3) + 2,  // Analog Input 1 Raw Input #2
    (155 << 3) + 2,  // Analog Input 1 Raw Input #3
    (157 << 3) + 2,  // Analog Input 1 Raw Input #4
    (159 << 3) + 2,  // Analog Input 1 Raw Input #5
    (161 << 3) + 4,  // Analog Input 1 Scaled Value #1
    (165 << 3) + 4,  // Analog Input 1 Scaled Value #2
    (169 << 3) + 4,  // Analog Input 1 Scaled Value #3
    (173 << 3) + 4,  // Analog Input 1 Scaled Value #4
    (177 << 3) + 4,  // Analog Input 1 Scaled Value #5
    (181 << 3) + 4,  // Analog Input 1 Min
    (185 << 3) + 4,  // Analog Input 1 Max
    (189 << 3) + 4,  // Analog Input 1 Low Warning
    (193 << 3) + 4,  // Analog Input 1 Low Alarm
    (197 << 3) + 4,  // Analog Input 1 High Warning
    (201 << 3) + 4,  // Analog Input 1 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (217 << 3) + 2,  // Analog Input 2 Selection
    (219 << 3) + 1,  // Analog Input 2 Function Select
    (220 << 3) + 2,  // Analog Input 2 Raw Input #1
    (222 << 3) + 2,  // Analog Input 2 Raw Input #2
    (224 << 3) + 2,  // Analog Input 2 Raw Input #3
    (226 << 3) + 2,  // Analog Input 2 Raw Input #4
    (228 << 3) + 2,  // Analog Input 2 Raw Input #5
    (230 << 3) + 4,  // Analog Input 2 Scaled Value #1
    (234 << 3) + 4,  // Analog Input 2 Scaled Value #2
    (238 << 3) + 4,  // Analog Input 2 Scaled Value #3
    (242 << 3) + 4,  // Analog Input 2 Scaled Value #4
    (246 << 3) + 4,  // Analog Input 2 Scaled Value #5
    (250 << 3) + 4,  // Analog Input 2 Min
    (254 << 3) + 4,  // Analog Input 2 Max
    (258 << 3) + 4,  // Analog Input 2 Low Warning
    (262 << 3) + 4,  // Analog Input 2 Low Alarm
    (266 << 3) + 4,  // Analog Input 2 High Warning
    (270 << 3) + 4,  // Analog Input 2 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (286 << 3) + 2,  // Analog Input 3 Selection
    (288 << 3) + 1,  // Analog Input 3 Function Select
    (289 << 3) + 2,  // Analog Input 3 Raw Input #1
    (291 << 3) + 2,  // Analog Input 3 Raw Input #2
    (293 << 3) + 2,  // Analog Input 3 Raw Input #3
    (295 << 3) + 2,  // Analog Input 3 Raw Input #4
    (297 << 3) + 2,  // Analog Input 3 Raw Input #5
    (299 << 3) + 4,  // Analog Input 3 Scaled Value #1
    (303 << 3) + 4,  // Analog Input 3 Scaled Value #2
    (307 << 3) + 4,  // Analog Input 3 Scaled Value #3
    (311 << 3) + 4,  // Analog Input 3 Scaled Value #4
    (315 << 3) + 4,  // Analog Input 3 Scaled Value #5
    (319 << 3) + 4,  // Analog Input 3 Min
    (323 << 3) + 4,  // Analog Input 3 Max
    (327 << 3) + 4,  // Analog Input 3 Low Warning
    (331 << 3) + 4,  // Analog Input 3 Low Alarm
    (335 << 3) + 4,  // Analog Input 3 High Warning
    (339 << 3) + 4,  // Analog Input 3 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (355 << 3) + 2,  // Analog Input 4 Selection
    (357 << 3) + 1,  // Analog Input 4 Function Select
    (358 << 3) + 2,  // Analog Input 4 Raw Input #1
    (360 << 3) + 2,  // Analog Input 4 Raw Input #2
    (362 << 3) + 2,  // Analog Input 4 Raw Input #3
    (364 << 3) + 2,  // Analog Input 4 Raw Input #4
    (366 << 3) + 2,  // Analog Input 4 Raw Input #5
    (368 << 3) + 4,  // Analog Input 4 Scaled Value #1
    (372 << 3) + 4,  // Analog Input 4 Scaled Value #2
    (376 << 3) + 4,  // Analog Input 4 Scaled Value #3
    (380 << 3) + 4,  // Analog Input 4 Scaled Value #4
    (384 << 3) + 4,  // Analog Input 4 Scaled Value #5
    (388 << 3) + 4,  // Analog Input 4 Min
    (392 << 3) + 4,  // Analog Input 4 Max
    (396 << 3) + 4,  // Analog Input 4 Low Warning
    (400 << 3) + 4,  // Analog Input 4 Low Alarm
    (404 << 3) + 4,  // Analog Input 4 High Warning
    (408 << 3) + 4,  // Analog Input 4 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (424 << 3) + 2,  // Analog Input 5 Selection
    (426 << 3) + 1,  // Analog Input 5 Function Select
    (427 << 3) + 2,  // Analog Input 5 Raw Input #1
    (429 << 3) + 2,  // Analog Input 5 Raw Input #2
    (431 << 3) + 2,  // Analog Input 5 Raw Input #3
    (433 << 3) + 2,  // Analog Input 5 Raw Input #4
    (435 << 3) + 2,  // Analog Input 5 Raw Input #5
    (437 << 3) + 4,  // Analog Input 5 Scaled Value #1
    (441 << 3) + 4,  // Analog Input 5 Scaled Value #2
    (445 << 3) + 4,  // Analog Input 5 Scaled Value #3
    (449 << 3) + 4,  // Analog Input 5 Scaled Value #4
    (453 << 3) + 4,  // Analog Input 5 Scaled Value #5
    (457 << 3) + 4,  // Analog Input 5 Min
    (461 << 3) + 4,  // Analog Input 5 Max
    (465 << 3) + 4,  // Analog Input 5 Low Warning
    (469 << 3) + 4,  // Analog Input 5 Low Alarm
    (473 << 3) + 4,  // Analog Input 5 High Warning
    (477 << 3) + 4,  // Analog Input 5 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (493 << 3) + 2,  // Analog Input 6 Selection
    (495 << 3) + 1,  // Analog Input 6 Function Select
    (496 << 3) + 2,  // Analog Input 6 Raw Input #1
    (498 << 3) + 2,  // Analog Input 6 Raw Input #2
    (500 << 3) + 2,  // Analog Input 6 Raw Input #3
    (502 << 3) + 2,  // Analog Input 6 Raw Input #4
    (504 << 3) + 2,  // Analog Input 6 Raw Input #5
    (506 << 3) + 4,  // Analog Input 6 Scaled Value #1
    (510 << 3) + 4,  // Analog Input 6 Scaled Value #2
    (514 << 3) + 4,  // Analog Input 6 Scaled Value #3
    (518 << 3) + 4,  // Analog Input 6 Scaled Value #4
    (522 << 3) + 4,  // Analog Input 6 Scaled Value #5
    (526 << 3) + 4,  // Analog Input 6 Min
    (530 << 3) + 4,  // Analog Input 6 Max
    (534 << 3) + 4,  // Analog Input 6 Low Warning
    (538 << 3) + 4,  // Analog Input 6 Low Alarm
    (542 << 3) + 4,  // Analog Input 6 High Warning
    (546 << 3) + 4,  // Analog Input 6 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (562 << 3) + 2,  // Analog Input 7 Selection
    (564 << 3) + 1,  // Analog Input 7 Function Select
    (565 << 3) + 2,  // Analog Input 7 Raw Input #1
    (567 << 3) + 2,  // Analog Input 7 Raw Input #2
    (569 << 3) + 2,  // Analog Input 7 Raw Input #3
    (571 << 3) + 2,  // Analog Input 7 Raw Input #4
    (573 << 3) + 2,  // Analog Input 7 Raw Input #5
    (575 << 3) + 4,  // Analog Input 7 Scaled Value #1
    (579 << 3) + 4,  // Analog Input 7 Scaled Value #2
    (583 << 3) + 4,  // Analog Input 7 Scaled Value #3
    (587 << 3) + 4,  // Analog Input 7 Scaled Value #4
    (591 << 3) + 4,  // Analog Input 7 Scaled Value #5
    (595 << 3) + 4,  // Analog Input 7 Min
    (599 << 3) + 4,  // Analog Input 7 Max
    (603 << 3) + 4,  // Analog Input 7 Low Warning
    (607 << 3) + 4,  // Analog Input 7 Low Alarm
    (611 << 3) + 4,  // Analog Input 7 High Warning
    (615 << 3) + 4,  // Analog Input 7 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (631 << 3) + 2,  // Analog Input 8 Selection
    (633 << 3) + 1,  // Analog Input 8 Function Select
    (634 << 3) + 2,  // Analog Input 8 Raw Input #1
    (636 << 3) + 2,  // Analog Input 8 Raw Input #2
    (638 << 3) + 2,  // Analog Input 8 Raw Input #3
    (640 << 3) + 2,  // Analog Input 8 Raw Input #4
    (642 << 3) + 2,  // Analog Input 8 Raw Input #5
    (644 << 3) + 4,  // Analog Input 8 Scaled Value #1
    (648 << 3) + 4,  // Analog Input 8 Scaled Value #2
    (652 << 3) + 4,  // Analog Input 8 Scaled Value #3
    (656 << 3) + 4,  // Analog Input 8 Scaled Value #4
    (660 << 3) + 4,  // Analog Input 8 Scaled Value #5
    (664 << 3) + 4,  // Analog Input 8 Min
    (668 << 3) + 4,  // Analog Input 8 Max
    (672 << 3) + 4,  // Analog Input 8 Low Warning
    (676 << 3) + 4,  // Analog Input 8 Low Alarm
    (680 << 3) + 4,  // Analog Input 8 High Warning
    (684 << 3) + 4,  // Analog Input 8 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (700 << 3) + 2,  // Analog Input 9 Selection
    (702 << 3) + 1,  // Analog Input 9 Function Select
    (703 << 3) + 2,  // Analog Input 9 Raw Input #1
    (705 << 3) + 2,  // Analog Input 9 Raw Input #2
    (707 << 3) + 2,  // Analog Input 9 Raw Input #3
    (709 << 3) + 2,  // Analog Input 9 Raw Input #4
    (711 << 3) + 2,  // Analog Input 9 Raw Input #5
    (713 << 3) + 4,  // Analog Input 9 Scaled Value #1
    (717 << 3) + 4,  // Analog Input 9 Scaled Value #2
    (721 << 3) + 4,  // Analog Input 9 Scaled Value #3
    (725 << 3) + 4,  // Analog Input 9 Scaled Value #4
    (729 << 3) + 4,  // Analog Input 9 Scaled Value #5
    (733 << 3) + 4,  // Analog Input 9 Min
    (737 << 3) + 4,  // Analog Input 9 Max
    (741 << 3) + 4,  // Analog Input 9 Low Warning
    (745 << 3) + 4,  // Analog Input 9 Low Alarm
    (749 << 3) + 4,  // Analog Input 9 High Warning
    (753 << 3) + 4,  // Analog Input 9 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (769 << 3) + 2,  // Analog Input 10 Selection
    (771 << 3) + 1,  // Analog Input 10 Function Select
    (772 << 3) + 2,  // Analog Input 10 Raw Input #1
    (774 << 3) + 2,  // Analog Input 10 Raw Input #2
    (776 << 3) + 2,  // Analog Input 10 Raw Input #3
    (778 << 3) + 2,  // Analog Input 10 Raw Input #4
    (780 << 3) + 2,  // Analog Input 10 Raw Input #5
    (782 << 3) + 4,  // Analog Input 10 Scaled Value #1
    (786 << 3) + 4,  // Analog Input 10 Scaled Value #2
    (790 << 3) + 4,  // Analog Input 10 Scaled Value #3
    (794 << 3) + 4,  // Analog Input 10 Scaled Value #4
    (798 << 3) + 4,  // Analog Input 10 Scaled Value #5
    (802 << 3) + 4,  // Analog Input 10 Min
    (806 << 3) + 4,  // Analog Input 10 Max
    (810 << 3) + 4,  // Analog Input 10 Low Warning
    (814 << 3) + 4,  // Analog Input 10 Low Alarm
    (818 << 3) + 4,  // Analog Input 10 High Warning
    (822 << 3) + 4,  // Analog Input 10 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (838 << 3) + 2,  // Analog Input 11 Selection
    (840 << 3) + 1,  // Analog Input 11 Function Select
    (841 << 3) + 2,  // Analog Input 11 Raw Input #1
    (843 << 3) + 2,  // Analog Input 11 Raw Input #2
    (845 << 3) + 2,  // Analog Input 11 Raw Input #3
    (847 << 3) + 2,  // Analog Input 11 Raw Input #4
    (849 << 3) + 2,  // Analog Input 11 Raw Input #5
    (851 << 3) + 4,  // Analog Input 11 Scaled Value #1
    (855 << 3) + 4,  // Analog Input 11 Scaled Value #2
    (859 << 3) + 4,  // Analog Input 11 Scaled Value #3
    (863 << 3) + 4,  // Analog Input 11 Scaled Value #4
    (867 << 3) + 4,  // Analog Input 11 Scaled Value #5
    (871 << 3) + 4,  // Analog Input 11 Min
    (875 << 3) + 4,  // Analog Input 11 Max
    (879 << 3) + 4,  // Analog Input 11 Low Warning
    (883 << 3) + 4,  // Analog Input 11 Low Alarm
    (887 << 3) + 4,  // Analog Input 11 High Warning
    (891 << 3) + 4,  // Analog Input 11 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (907 << 3) + 2,  // Tach Input 1 Selection
    (909 << 3) + 2,  // Tach Input 1 Pulses Per Revolution
    (911 << 3) + 2,  // Tach Input 1 Min
    (913 << 3) + 2,  // Tach Input 1 Max
    (915 << 3) + 2,  // Tach Input 1 Low Warning
    (917 << 3) + 2,  // Tach Input 1 Low Alarm
    (919 << 3) + 2,  // Tach Input 1 High Warning
    (921 << 3) + 2,  // Tach Input 1 High Alarm
    (923 << 3) + 2,  // Tach Input 1 Low Restriction 1
    (925 << 3) + 2,  // Tach Input 1 High Restriction 1
    (927 << 3) + 2,  // Tach Input 1 Low Restriction 2
    (929 << 3) + 2,  // Tach Input 1 High Restriction 2
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (945 << 3) + 2,  // Tach Input 2 Selection
    (947 << 3) + 2,  // Tach Input 2 Pulses Per Revolution
    (949 << 3) + 2,  // Tach Input 2 Min
    (951 << 3) + 2,  // Tach Input 2 Max
    (953 << 3) + 2,  // Tach Input 2 Low Warning
    (955 << 3) + 2,  // Tach Input 2 Low Alarm
    (957 << 3) + 2,  // Tach Input 2 High Warning
    (959 << 3) + 2,  // Tach Input 2 High Alarm
    (961 << 3) + 2,  // Tach Input 2 Low Restriction 1
    (963 << 3) + 2,  // Tach Input 2 High Restriction 1
    (965 << 3) + 2,  // Tach Input 2 Low Restriction 2
    (967 << 3) + 2,  // Tach Input 2 High Restriction 2
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (983 << 3) + 2,  // Flow Input 1 Selection
    (985 << 3) + 2,  // Flow Input 1 K Factor
    (987 << 3) + 2,  // Flow Input 1 Min
    (989 << 3) + 2,  // Flow Input 1 Max
    (991 << 3) + 2,  // Flow Input 1 Low Warning
    (993 << 3) + 2,  // Flow Input 1 Low Alarm
    (995 << 3) + 2,  // Flow Input 1 High Warning
    (997 << 3) + 2,  // Flow Input 1 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (1013 << 3) + 2,  // Flow Input 2 Selection
    (1015 << 3) + 2,  // Flow Input 2 K Factor
    (1017 << 3) + 2,  // Flow Input 2 Min
    (1019 << 3) + 2,  // Flow Input 2 Max
    (1021 << 3) + 2,  // Flow Input 2 Low Warning
    (1023 << 3) + 2,  // Flow Input 2 Low Alarm
    (1025 << 3) + 2,  // Flow Input 2 High Warning
    (1027 << 3) + 2,  // Flow Input 2 High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (1043 << 3) + 2,  // CHT Min
    (1045 << 3) + 2,  // CHT Max
    (1047 << 3) + 2,  // CHT Low Warning
    (1049 << 3) + 2,  // CHT Low Alarm
    (1051 << 3) + 2,  // CHT High Warning
    (1053 << 3) + 2,  // CHT High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (1069 << 3) + 2,  // EGT Min
    (1071 << 3) + 2,  // EGT Max
    (1073 << 3) + 2,  // EGT Low Warning
    (1075 << 3) + 2,  // EGT Low Alarm
    (1077 << 3) + 2,  // EGT High Warning
    (1079 << 3) + 2,  // EGT High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (1095 << 3) + 2,  // CHT Rate Of Change Min
    (1097 << 3) + 2,  // CHT Rate Of Change Max
    (1099 << 3) + 2,  // CHT Rate Of Change High Warning
    (1101 << 3) + 2,  // CHT Rate Of Change High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (1117 << 3) + 2,  // CHT Deviation Min
    (1119 << 3) + 2,  // CHT Deviation Max
    (1121 << 3) + 2,  // CHT Deviation High Warning
    (1123 << 3) + 2,  // CHT Deviation High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (1139 << 3) + 2,  // EGT Rate Of Change Min
    (1141 << 3) + 2,  // EGT Rate Of Change Max
    (1143 << 3) + 2,  // EGT Rate Of Change High Warning
    (1145 << 3) + 2,  // EGT Rate Of Change High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (1161 << 3) + 2,  // EGT Deviation Min
    (1163 << 3) + 2,  // EGT Deviation Max
    (1165 << 3) + 2,  // EGT Deviation High Warning
    (1167 << 3) + 2,  // EGT Deviation High Alarm
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (0 << 3) + 0,  //
    (1183 << 3) + 2,  // CHT 1 Calibration Offset
    (1185 << 3) + 2,  // CHT 2 Calibration Offset
    (1187 << 3) + 2,  // CHT 3 Calibration Offset
    (1189 << 3) + 2,  // CHT 4 Calibration Offset
    (1191 << 3) + 2,  // CHT 5 Calibration Offset
    (1193 << 3) + 2,  // CHT 6 Calibration Offset
    (1195 << 3) + 2,  // EGT 1 Calibration Offset
    (1197 << 3) + 2,  // EGT 2 Calibration Offset
    (1199 << 3) + 2,  // EGT 3 Calibration Offset
    (1201 << 3) + 2,  // EGT 4 Calibration Offset
    (1203 << 3) + 2,  // EGT 5 Calibration Offset
    (1205 << 3) + 2,  // EGT 6 Calibration Offset
};

void Config::setFlash(FRAM_SPI *f) {
    flash = f;
}

/* This function reads the configuration map from the
   program memory array above and writes the private
   addr and len values.  If a key is invalid it writes
   0x00 to len. */
void Config::getAddr(uint16_t key) {
    unsigned int result;
    if(key > 371) { // Key out of bounds
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

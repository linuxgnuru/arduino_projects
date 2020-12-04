/*
 * millis
 */
unsigned long lastMillisA = 0;
unsigned long lastMillisB = 0;
unsigned long lastMillisBlink = 0;
unsigned long aMillis = 0;
unsigned long bMillis = 0;
unsigned long lastMillis_led = 0;

/*
  data0 and data1 = map (6x3) 0-7 = data0, 8-15 = data1, 16 = data2[0], 17 = data3[0]
  data2 and data3 = side 0-6
  You can not touch bit 0 or 1 on data2 or data3, 0 is used for
  example: bitSet(data[2], (2-7), 1/0);
*/

byte data[4];

const byte flashers[] = {
  0b00001000,
  0b00100000,
  0b10000000,
  0b00000100,
  0b00010000,
  0b01000000
};

/*
   SPI.transfer(0b00000000); <-- 0
   SPI.transfer(0b00000000); <-- 1
   SPI.transfer(0b00000000); <-- 2
   SPI.transfer(0b00000000); <-- 3

     01234567
   0b00000000 <-- 0-5 = right LEDs, 6 is unused, 7 = 1 on grid
   0b00000000 <-- 0-5 = left  LEDs, 6 is unused, 7 = 0 on grid
   0b00000000 <-- 0-7 = 02 - 09; 20 - 27
   0b00000000 <-- 0-7 = 10 - 17; 30 - 37

    17 = chip 1, pin 7; 35 = chip 3, pin 5
    
    00|01|02|03|04|05
   +--+--+--+--+--+--+
00 |17|07|20|21|22|23|
---+--+--+--+--+--+--+
01 |24|25|26|27|30|31|
---+--+--+--+--+--+--+
02 |32|33|34|35|36|37|
---+--+--+--+--+--+--+

    00|01|02|03|04|05
   +--+--+--+--+--+--+
00 |00|01|02|03|04|05|
---+--+--+--+--+--+--+
01 |06|07|08|09|10|11|
---+--+--+--+--+--+--+
02 |12|13|14|15|16|17|
---+--+--+--+--+--+--+

*/

const byte grid[] = {
  0b11110000, // 00, really 7 on data[1]
  0b11110000, // 01, really 7 on data[0]

  0b10000000, // 02, chip 2, pin 0, (0, 2)
  0b01000000, // 03, chip 2, pin 1, (0, 3)
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  0b00000010,
  0b00000001,

  0b10000000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  0b00000010,
  0b00000001
};


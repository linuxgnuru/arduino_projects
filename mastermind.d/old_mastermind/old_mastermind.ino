
/*
  int ctr = 0;
 for (i = 0; i < 12; i++)
 {
 if (i < 9)
 data1 ^= 
 else
 {
 data1 = 0b00000000;
 data2 |= 1 << ctr;
 }
 digitalWrite(latchPin, 0);
 shiftOut(dataPin, clockPin, data1);
 shiftOut(dataPin, clockPin, data2);
 digitalWrite(latchPin, 1);
 if (i < 9)
 data1 &= ~(1 << i);
 else
 {
 data2 &= ~(1 << ctr);
 ctr++;
 }
 digitalWrite(latchPin, 0);
 shiftOut(dataPin, clockPin, data1);
 shiftOut(dataPin, clockPin, data2);
 digitalWrite(latchPin, 1);
 }
 */
/*
  for (i = 0; i < 16; i++)
 {
 data1 = dataArray1[i];
 data2 = dataArray2[i];
 digitalWrite(latchPin, 0);
 shiftOut(dataPin, clockPin, data1);
 shiftOut(dataPin, clockPin, data2);
 digitalWrite(latchPin, 1);
 delay(500);
 }
 */

/*
  switch (testPin(aPin1))
 {
 case LED_R:
 digitalWrite(8, HIGH);
 for (i = 9; i < 13; i++)
 digitalWrite(i, LOW);
 break;
 case LED_B:
 digitalWrite(8, LOW);
 digitalWrite(9, HIGH);
 for (i = 10; i < 13; i++)
 digitalWrite(i, LOW);
 break;
 case LED_G:
 digitalWrite(8, LOW);
 digitalWrite(9, LOW);
 digitalWrite(10, HIGH);
 for (i = 11; i < 13; i++)
 digitalWrite(i, LOW);
 break;
 case LED_W:
 for (i = 8; i < 11; i++)
 digitalWrite(i, LOW);
 digitalWrite(11, HIGH);
 digitalWrite(12, LOW);
 break;
 case LED_Y:
 for (i = 8; i < 12; i++)
 digitalWrite(i, LOW);
 digitalWrite(12, HIGH);
 break;
 case LED_0:
 for (i = 8; i < 13; i++)
 digitalWrite(i, LOW);
 break;
 default:  // Should never get here.
 for (i = 8; i < 13; i++)
 digitalWrite(i, HIGH);
 break;
 }
 */
/*
  data = digitArray[last];
 digitalWrite(latchPin, 0);
 //move 'em out
 shiftOut(dataPin, clockPin, data);
 //return the latch pin high to signal chip that it
 //no longer needs to listen for information
 digitalWrite(latchPin, 1);
 */

/*
  switch (testPin(A0))
 {
 case LED_R: 
 Serial.println("red"); 
 break;
 case LED_B: 
 Serial.println("blue"); 
 break;
 case LED_G: 
 Serial.println("green"); 
 break;
 case LED_W: 
 Serial.println("white"); 
 break;
 case LED_Y: 
 Serial.println("yellow"); 
 break;
 case LED_0: 
 Serial.println("nothing"); 
 break;
 }
 */

/*
byte dataArray1[] = {
 0b10000000, // g00
 0b01000000,
 0b00100000,
 0b00010000,
 0b00001000,
 0b00000100,
 0b00000010,
 0b00000001
 };
 
 byte dataArray2[] = {
 0b00001000,
 0b00000100,
 0b00000010,
 0b00000001,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000
 };
 */




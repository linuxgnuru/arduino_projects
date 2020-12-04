const byte SPI_LATCH = 10;

// 0 = left, 1 = right
const byte anaPins[] = { A1, A2 };

// dPins 0 will be the first
const byte dPins[2][3] = {
  { 4, 3, 2 },
  { 7, 6, 5 }
};

// not pins but the index for the left and right status leds
// e.g.
// led(l_r[LEFT][0], LED_ON, BOARD_0);
const byte l_r[2][3] = {
  { 2,  9, 1 },
  { 8, 10, 0 }
};


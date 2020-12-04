#include <Arduboy2.h>

Arduboy2 arduboy;

int playerx = 5;
int playery = 10;

const unsigned char background[] PROGMEM  = {
  0x84, 0x20, 0x9, 0x00, 0x24, 0x00, 0x10, 0x80,
};

const unsigned char player[] PROGMEM  = {
  0xfe, 0x1,  0x3d, 0x25, 0x25, 0x3d, 0x1,  0x1,
  0xc1, 0x1,  0x3d, 0x25, 0x25, 0x3d, 0x1,  0xfe,
  0x7f, 0x80, 0x9c, 0xbc, 0xb0, 0xb0, 0xb2, 0xb2,
  0xb3, 0xb0, 0xb0, 0xb0, 0xbc, 0x9c, 0x80, 0x7f,
};

void setup()
{
  arduboy.begin();
  arduboy.clear();
}

void loop()
{
  arduboy.clear();
  if (arduboy.pressed(LEFT_BUTTON) && playerx > 0) playerx--;
  if (arduboy.pressed(RIGHT_BUTTON) && playerx < WIDTH - 16) playerx++;
  if (arduboy.pressed(UP_BUTTON) && playery > 0)   playery--;
  if (arduboy.pressed(DOWN_BUTTON) && playery < HEIGHT - 16)  playery++;
  // For each column on the screen
  for (int backgroundx = 0; backgroundx < 128; backgroundx = backgroundx + 8)
  {
    // For each row in the column
    for (int backgroundy = 0; backgroundy < 64; backgroundy = backgroundy + 8)
    {
      // Draw a background tile
      arduboy.drawBitmap(backgroundx, backgroundy, background, 8, 8, WHITE);
    }
  }
  // Draw black square
  arduboy.fillRect(playerx, playery, 16, 16, BLACK);
  // Draw player sprite
  arduboy.drawBitmap(playerx, playery, player, 16, 16, WHITE);
  arduboy.display();
}


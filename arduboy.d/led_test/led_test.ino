#include <Arduboy2.h>

Arduboy2 arduboy;

typedef enum {
  LED_B,
  LED_G,
  LED_R
} led_enum;


byte x;
byte y;

byte led_val[3] = { 0, 0, 0 };
byte led_nums[3] = { BLUE_LED, GREEN_LED, RED_LED };

byte cur_led = LED_B;

#define LED_MAX 255

//char title[] = "Press Buttons!";
//char title[] = "  |  \n--+--\n  |  \n";
char title[] = "*";
#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define NUM_CHARS (sizeof(title) - 1)
#define X_MAX (WIDTH - (NUM_CHARS * CHAR_WIDTH) + 1)
#define Y_MAX (HEIGHT - CHAR_HEIGHT)

boolean flash_light = false;
byte tmp = 0;

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  x = (WIDTH / 2) - (NUM_CHARS * CHAR_WIDTH / 2);
  y = (HEIGHT / 2) - (CHAR_HEIGHT / 2);
  for (int i = 0; i < 3; i++)
    pinMode(led_nums[i], OUTPUT);
}

void loop()
{
  if (!(arduboy.nextFrame()))
    return;
  if (arduboy.pressed(RIGHT_BUTTON) && (x < X_MAX)) x++;
  if (arduboy.pressed(LEFT_BUTTON) && (x > 0))      x--;
  if (arduboy.pressed(UP_BUTTON) && (y > 0))        y--;
  if (arduboy.pressed(DOWN_BUTTON) && (y < Y_MAX))  y++;
  if (arduboy.pressed(A_BUTTON) && !arduboy.pressed(B_BUTTON))
  {
    if (led_val[cur_led] + 1 == 256)
      led_val[cur_led] = 0;
    //analogWrite(led_nums[cur_led], led_val[cur_led]++);
    analogWrite(led_nums[cur_led], led_val[cur_led]);
    led_val[cur_led]++;
    //analogWrite(led_nums[cur_led], 127);
  }
  if (arduboy.pressed(B_BUTTON) && !arduboy.pressed(A_BUTTON))
  {
    digitalWrite(led_nums[cur_led], HIGH);
    cur_led++;
    if (cur_led > LED_R)
      cur_led = LED_B;
  }
  if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON))
  {
    if (flash_light)
    {
      flash_light = false;
      tmp = 255;
    }
    else
    {
      flash_light = true;
      tmp = 0;
    }
    for (int i = 0; i < 3; i++)
    {
      analogWrite(led_nums[i], tmp);
    }
  }
#if 0
  if (arduboy.pressed(A_BUTTON) && b_val) && (b_val < LED_MAX)
  {
    if (b_val + 1 > 255)
      b_val = -1;
    analogWrite(BLUE_LED, ++b_val);
  }
  if (arduboy.pressed(B_BUTTON))
  {
    analogWrite(BLUE_LED, b_val);
    if (--b_val < 0)
      b_val = 255;
  }
#endif
  arduboy.clear();
  // we set our cursor x pixels to the right and y down from the top
  arduboy.setCursor(x, y);
  // then we print to screen what is stored in our title variable we declared earlier
  arduboy.print(title);
  arduboy.display();
}


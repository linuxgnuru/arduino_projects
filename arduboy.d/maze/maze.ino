/*
  Dumb dungeon rendering code for the arduboy, made by @bakagamedev on twitter. If you find any bugs, you can contact me there.
  Has no dependencies beyond the arduboy library itself and a compatible IDE.
  Licenced under wtfpl, http://www.wtfpl.net/about/

  Enjoy!

  Changelog:
  1.0 (9th june 2016)
  Initial release
  1.1 (11th june 2016)
  Added backstepping using down button
  Changed map view to left side of screen and made the B button swap rendering mode
  Tidied here and there
*/
#include "Arduboy2.h"
Arduboy2 ab;

// 3d view or map?
// don't ask, it doesn't compile correctly with a bool and I cannot for the life of me understand why,
// so this is a stupid stopgap until I figure that out
//int viewMap = 1001;
boolean viewMap = true;
// list of visible walls
bool wallShow[11] = { };
// this maze should probably be in PROGMEM if you want it to be static, but the intention here is to integrate it into a generator
bool maze[8][8] =
{
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 1, 0, 0, 1,
  1, 1, 0, 1, 0, 0, 1, 1,
  1, 0, 0, 0, 0, 1, 0, 1,
  1, 1, 0, 1, 1, 1, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 1, 1, 1, 1, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

uint8_t cameraX = 1;
uint8_t cameraY = 1;
int8_t cameraDir = 0;

void calculateView()
{
  wallShow[10] = false; // [10] is the players current position, don't bother checking there.
  // Searches the grid and fills a visibility array with whatever it finds.
  // This should probably be automated with a fancy for loop.
  switch (cameraDir)
  {
    case 0:
      wallShow[0] = wallCheck(cameraX + 3, cameraY - 1);
      wallShow[1] = wallCheck(cameraX + 3, cameraY);
      wallShow[2] = wallCheck(cameraX + 3, cameraY + 1);
      wallShow[3] = wallCheck(cameraX + 2, cameraY - 1);
      wallShow[4] = wallCheck(cameraX + 2, cameraY);
      wallShow[5] = wallCheck(cameraX + 2, cameraY + 1);
      wallShow[6] = wallCheck(cameraX + 1, cameraY - 1);
      wallShow[7] = wallCheck(cameraX + 1, cameraY);
      wallShow[8] = wallCheck(cameraX + 1, cameraY + 1);
      wallShow[9] = wallCheck(cameraX, cameraY - 1);
      wallShow[11] = wallCheck(cameraX, cameraY + 1);
      break;
    case 1:
      wallShow[0] = wallCheck(cameraX + 1, cameraY + 3);
      wallShow[1] = wallCheck(cameraX, cameraY + 3);
      wallShow[2] = wallCheck(cameraX - 1, cameraY + 3);
      wallShow[3] = wallCheck(cameraX + 1, cameraY + 2);
      wallShow[4] = wallCheck(cameraX, cameraY + 2);
      wallShow[5] = wallCheck(cameraX - 1, cameraY + 2);
      wallShow[6] = wallCheck(cameraX + 1, cameraY + 1);
      wallShow[7] = wallCheck(cameraX, cameraY + 1);
      wallShow[8] = wallCheck(cameraX - 1, cameraY + 1);
      wallShow[9] = wallCheck(cameraX + 1, cameraY);
      wallShow[11] = wallCheck(cameraX - 1, cameraY);
      break;
    case 2:
      wallShow[0] = wallCheck(cameraX - 3, cameraY + 1);
      wallShow[1] = wallCheck(cameraX - 3, cameraY);
      wallShow[2] = wallCheck(cameraX - 3, cameraY - 1);
      wallShow[3] = wallCheck(cameraX - 2, cameraY + 1);
      wallShow[4] = wallCheck(cameraX - 2, cameraY);
      wallShow[5] = wallCheck(cameraX - 2, cameraY - 1);
      wallShow[6] = wallCheck(cameraX - 1, cameraY + 1);
      wallShow[7] = wallCheck(cameraX - 1, cameraY);
      wallShow[8] = wallCheck(cameraX - 1, cameraY - 1);
      wallShow[9] = wallCheck(cameraX, cameraY + 1);
      wallShow[11] = wallCheck(cameraX, cameraY - 1);
      break;
    case 3:
      wallShow[0] = wallCheck(cameraX - 1, cameraY - 3);
      wallShow[1] = wallCheck(cameraX, cameraY - 3);
      wallShow[2] = wallCheck(cameraX + 1, cameraY - 3);
      wallShow[3] = wallCheck(cameraX - 1, cameraY - 2);
      wallShow[4] = wallCheck(cameraX, cameraY - 2);
      wallShow[5] = wallCheck(cameraX + 1, cameraY - 2);
      wallShow[6] = wallCheck(cameraX - 1, cameraY - 1);
      wallShow[7] = wallCheck(cameraX, cameraY - 1);
      wallShow[8] = wallCheck(cameraX + 1, cameraY - 1);
      wallShow[9] = wallCheck(cameraX - 1, cameraY);
      wallShow[11] = wallCheck(cameraX + 1, cameraY);
      break;
    default:
      break;
  }
  if (wallShow[7])  // speed up by disabling hidden walls
  {
    wallShow[4] = false;
    wallShow[0] = false;
    wallShow[2] = false;
  }
  if (wallShow[4])
  {
    wallShow[1] = false;
  }
}

bool wallCheck(int x, int y)
{
  if ((x < 0) or (y < 0) or (x > 7) or (y > 7))
    return (true);  // Out of bounds is full of walls, means you don't have to waste space by surrounding a map
  else
    return (maze[x][y]);
}

void drawView() //byte x,byte width)
{
  /*
    draws the viewpoint as calculated in calculateView(), drawing every wall in turn, from far->near.
  */
  int wallSize[] = { 6, 10, 18, 32, 64}; //size in pixels of each step
  char wall = 0;  //current wall

  int drawSize, halfSize, backSize, halfBackSize, left, leftBack, top, topBack;
  for (char i = 0; i < 4; i++) //distance
  {
    drawSize = wallSize[i + 1]; halfSize = drawSize / 2;    // size of walls on screen
    backSize = wallSize[i];   halfBackSize = backSize / 2;  // size of the backside of the walls, for depth
    leftBack = 32 - (halfBackSize * 3);                     // x position of the walls on screen
    left     = 32 - (halfSize * 3);
    topBack  = 32 - halfBackSize;                           // y position of the walls on screen
    top      = 32 - halfSize;

    for (char n = 0; n < 3; n++) //left->right
    {
      if (wallShow[wall]) // if wall exists, draw it
      {
        if ((n == 0) && (!wallShow[wall + 1])) //left wall, only draw if the middle wall is missing
        {
          ab.fillRect(left + drawSize, top, (leftBack + backSize) - (left + drawSize), drawSize, 0); //blank out area behind wall
          ab.drawLine(leftBack + backSize, topBack + backSize, left + drawSize, top + drawSize, 1); //lower line
          ab.drawLine(left + drawSize, top, leftBack + backSize, topBack, 1);             //upper line
          ab.drawLine(leftBack + backSize, topBack, leftBack + backSize, topBack + backSize, 1); //far line
        }
        if ((n == 2) && (!wallShow[wall - 1])) //right wall, ditto
        {
          ab.fillRect(leftBack, top, left - leftBack, drawSize, 0);
          ab.drawLine(leftBack, topBack, left, top, 1); //upper
          ab.drawLine(leftBack, topBack + backSize, left, top + drawSize, 1); //lower
          ab.drawLine(leftBack, topBack, leftBack, topBack + backSize, 1); //side
        }
        //draw flat wall if not immediately next to the camera, and if there is no wall infront
        if ((i < 3) && (!wallShow[wall + 3]))
        {
          int wid = drawSize; //width of wall
          if ((n == 2) && (left + wid > 64)) //if the wall goes off the render area, chop the width down
          {
            //magic numbering this because the only time it ever happens is on a close right side wall
            wid = 15; //(64-halfSize)-1;
          }
          ab.fillRect(left, top, wid, drawSize, 0); //blank out wall area and draw then draw the outline
          ab.drawRect(left, top, wid + 1, drawSize + 1, 1);
        }
      }
      wall++;
      left += drawSize;     //advance left positions
      leftBack += backSize;
    }
  }
  ab.drawRect(0, 0, 64, 64, 1);
}

void drawMap()
{
  //draw map grid
  const uint8_t dx = 0;  //x offset, puts on right side of the screen
  for (int iy = 0; iy < 8; iy++) //loops x&y, draws a rectangle for every wall
  {
    for (int ix = 0; ix < 8; ix++)
    {
      if (wallCheck(ix, iy))
      {
        ab.drawRect(dx + (ix * 8), (iy * 8), 9, 9, 1);
      }
    }
  }
  //draws the player as a cross
  char cx = cameraX + 1;
  char cy = cameraY + 1;
  ab.drawLine(dx + (((cx) * 8) - 6), (cy * 8) - 6, dx + ((cx * 8) - 2), (cy * 8) - 2, 1);
  ab.drawLine(dx + (((cx) * 8) - 6), (cy * 8) - 2, dx + ((cx * 8) - 2), (cy * 8) - 6, 1);

  //outlines the map
  ab.drawLine(dx + 64, 0, dx + 64, 63, 1);
  ab.drawLine(dx, 63, dx + 64, 63, 1);
}

void initView()
{
  //viewMap = 1001;
  viewMap = true;
}

void setup()
{
  ab.begin();
  ab.setFrameRate(1); //Kludge around not bothering to implement a keydown function
  calculateView();
}

void loop()
{
  if (!(ab.nextFrame()))
    return;

  //change view angle on press
  char cd = cameraDir;
  if (ab.pressed(LEFT_BUTTON))
    cameraDir -= 1;
  if (ab.pressed(RIGHT_BUTTON))
    cameraDir += 1;

  if (cameraDir < 0)
    cameraDir = 3;
  if (cameraDir > 3)
    cameraDir = 0;
  if (cameraDir != cd)
    calculateView();

  //if the player presses up or down and the space is free, go to it
  if (ab.pressed(UP_BUTTON) or (ab.pressed(DOWN_BUTTON)))
  {
    //calculate next space
    int8_t nx = 0, ny = 0; //using int8 rather than uint8 so negative numbers are possible
    if (cameraDir == 0)
    {
      nx = 1;
    }
    if (cameraDir == 1)
    {
      ny = 1;
    }
    if (cameraDir == 2)
    {
      nx = -1;
    }
    if (cameraDir == 3)
    {
      ny = -1;
    }

    if (ab.pressed(DOWN_BUTTON))  //if they pressed down, flip the direction
    {
      nx = 0 - nx;
      ny = 0 - ny;
    }

    nx += cameraX;  //calculate new coordinate
    ny += cameraY;
    if (!wallCheck(nx, ny)) //if space is empty and in bounds, move to it
    {
      cameraX = nx;
      cameraY = ny;
      calculateView();
    }
  }

  if (ab.pressed(B_BUTTON)) //swap screen mode when B button is pressed
  {
    viewMap = !viewMap;
    #if 0
    if (viewMap == 1001) //again, stupid stopgap, using 1001 and 1000 instead of true/false because that doesn't compile correctly.
      viewMap = 1000;
    else
      viewMap = 1001;
      #endif
  }

  ab.clear(); //clear screen

  //if (viewMap == 1001)
  if (viewMap)
  {
    drawMap();  //draw map
  }
  else
  {
    drawView(); //draw perspective
    //draw camera angle indicator, for debugging.
    for (char i = 0; i < cameraDir + 1; i++)
    {
      ab.drawLine(30 + (i * 2), 0, 30 + (i * 2), 4, 1);
    }
  }

  ab.display();
}


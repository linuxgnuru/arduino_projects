#include "maze.h"

const int buttonPin = 8;
int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long lm = 0;

Maze newMaze;

uint8_t leveldata[64] =
{
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 1, 0, 0,
  1, 1, 0, 1, 1, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 1, 0,
  1, 1, 0, 1, 1, 1, 1, 0,
  1, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 0
};

/*
  typedef struct
  {
  int x, y;
  void *p;
  int c;
  char dirs;
  } Node;

  Node *nodes;
  const int width  = 9;
  const int height = 9;
*/
void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(A1));
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  //makeMap();
}

void loop()
{
  unsigned long cm = millis();
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState)
    lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == LOW)
      {
        makeMaze();
      }
    }
  }
  if (cm - lm >= 1000)
  {
    lm = cm;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  lastButtonState = reading;
}

void makeMaze()
{
  //newMaze = new Maze;
  newMaze.begin(9, 9);
  newMaze.init();
  if (newMaze.start())
  {
    Serial.println(F("ERROR"));
  }
  else
  {
    newMaze.create(leveldata);
    printStuff();
  }
  //delete newMaze;
}

#if 0
void map_init()
{
  static Node *n;

  nodes = new Node[width * height];
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      n = nodes + i + j * width;
      if (i * j % 2)
      {
        n->x = i;
        n->y = j;
        n->dirs = 15;
        n->c = 0;
      }
      else
        n->c = 1;
    }
  }
}

Node *link(Node *n)
{
  int x = 0, y = 0;
  char dir;
  Node *dest;
  if (n == NULL)
    return NULL;
  while (n->dirs)
  {
    dir = (1 << random(4));
    if (~n->dirs & dir)
      continue;
    n->dirs &= ~dir;
    switch (dir)
    {
      case 1:
        if (n->x + 2 < width)
        {
          x = n->x + 2;
          y = n->y;
        }
        else
          continue;
        break;
      case 2:
        if (n->y + 2 < height)
        {
          x = n->x;
          y = n->y + 2;
        }
        else
          continue;
        break;
      case 4:
        if (n->x - 2 >= 0)
        {
          x = n->x - 2;
          y = n->y;
        }
        else
          continue;
        break;
      case 8:
        if (n->y - 2 >= 0)
        {
          x = n->x;
          y = n->y - 2;
        }
        else
          continue;
        break;
    }
    dest = nodes + x + y * width;
    if (dest->c == 0)
    {
      if (dest->p != NULL)
        continue;
      dest->p = n;
      nodes[n->x + (x - n->x) / 2 + (n->y + (y - n->y) / 2) * width].c = 0;
      return dest;
    }
  }
  return (Node *)n->p;
}

void makeMap()
{
  static Node *start, *last;

  if (nodes != NULL)
    delete nodes;
  map_init();
  start = nodes + 1 + width;
  start->p = start;
  last = start;
  while ((last = link(last)) != start);
  Serial.println();
  draw();
}


void draw()
{
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (nodes[j + i * width].c == 1)
        Serial.print(F("1"));
      else
        Serial.print(F(" "));
    }
    Serial.println();
  }
}
#endif

void printStuff()
{
  for (int i = 1; i < 65; i++)
  {
    Serial.print(leveldata[i - 1]);
    if (i % 8 == 0)
    {
      if (i != 64)
        Serial.println(F(","));
    }
    else
    {
      Serial.print(F(", "));
    }
  }
  Serial.println();
}


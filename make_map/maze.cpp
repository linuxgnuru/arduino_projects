#include <Arduino.h>

#include "node.h"
#include "maze.h"

void Maze::begin(int w, int h)
{
  is_init = false;
  width = w;
  height = h;
};

void Maze::init(void)
{
  if (!is_init)
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
    is_init = true;
  }
}

Node* Maze::link(Node *n)
{
  int x = 0, y = 0;
  char dir;
  Node *dest;

  if (n == NULL)
    return NULL;
  while (n->dirs)
  {
    dir = (1 << (rand() % 4));
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
      if (dest->parent != NULL)
        continue;
      dest->parent = n;
      nodes[n->x + (x - n->x) / 2 + (n->y + (y - n->y) / 2) * width].c = 0;
      return dest;
    }
  }
  return (Node *)n->parent;
}

void Maze::create(uint8_t leveldata[64])
{
  int ctr = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      leveldata[ctr++] = nodes[j + i * width].c;
    }
  }
  cleanup();
}

/*
  void Maze::draw()
  {
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Serial.print("%c", (nodes[j + i * width].c ? '#' : ' '));
        }
        Serial.println();
    }
  }
*/

bool Maze::start(void)
{
  static Node *start, *last;

  if (!is_init)
    return true;
  start = nodes + 1 + width;
  start->parent = start;
  last = start;
  while ((last = link(last)) != start);
  return false;
}

void Maze::cleanup(void)
{
  if (nodes != NULL)
    delete nodes;
}


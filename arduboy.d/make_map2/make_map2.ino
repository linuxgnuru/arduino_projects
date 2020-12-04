#include <vector>
#include <list>

const int maze_size_x = 8;
const int maze_size_y = 8;

vector < vector < bool > > maze;
list < pair < int, int> > drillers;

unsigned long lastm = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  makeMap();
  printMap();
}

void loop()
{
  unsigned long cm = millis();

  if (cm - lastm >= 1000)
  {
    lastm = cm;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN);
  }
}

void makeMap()
{
  maze.resize(maze_size_y);
  for (size_t y = 0; y < maze_size_y; y++)
  {
    maze[y].resize(maze_size_x);
  }
  for (size_t x = 0; x < maze_size_x; x++)
  {
    for (size_t y = 0; y < maze_size_y; y++)
    {
      maze[y][x] = false;
    }
  }
  drillers.push_back(make_pair(maze_size_x / 2, maze_size_y / 2));
  while (drillers.size() > 0)
  {
    list < pair < int, int> >::iterator m, _m, temp;
    m = drillers.begin();
    _m = drillers.end();
    while (m != _m)
    {
      bool remove_driller = false;
      switch (rand() % 4)
      {
        case 0:
          (*m).second -= 2;
          if ((*m).second < 0 || maze[(*m).second][(*m).first])
          {
            remove_driller = true;
            break;
          }
          maze[(*m).second + 1][(*m).first] = true;
          break;
        case 1:
          (*m).second += 2;
          if ((*m).second >= maze_size_y || maze[(*m).second][(*m).first])
          {
            remove_driller = true;
            break;
          }
          maze[(*m).second - 1][(*m).first] = true;
          break;
        case 2:
          (*m).first -= 2;
          if ((*m).first < 0 || maze[(*m).second][(*m).first])
          {
            remove_driller = true;
            break;
          }
          maze[(*m).second][(*m).first + 1] = true;
          break;
        case 3:
          (*m).first += 2;
          if ((*m).first >= maze_size_x || maze[(*m).second][(*m).first])
          {
            remove_driller = true;
            break;
          }
          maze[(*m).second][(*m).first - 1] = true;
          break;
      }
      if (remove_driller)
        m = drillers.erase(m);
      else
      {
        drillers.push_back(make_pair((*m).first, (*m).second));
        // uncomment the line below to make the maze easier
        // if (rand()%2)
        drillers.push_back(make_pair((*m).first, (*m).second));
        maze[(*m).second][(*m).first] = true;
        ++m;
      }
    }
  }
}

void printMap()
{
  for (size_t y = 0; y < maze_size_y; y++)
  {
    for (size_t x = 0; x < maze_size_x; x++)
    {
      Serial.print(!maze[y][x]));
      Serial.print(F(", "));
    }
    Serial.println();
  }
}


#pragma once

#include "node.h"

class Maze {
  private:
    int8_t width;
    int8_t height;
    bool is_init;
    Node *nodes;

    Node *link(Node *n);
    void cleanup(void);
    
  public:
    Maze() { }
    ~Maze() { cleanup(); }
    bool start();
    void begin(int w, int h);
    void init(void);
    void create(uint8_t leveldata[64]);
    //void draw(void);
};



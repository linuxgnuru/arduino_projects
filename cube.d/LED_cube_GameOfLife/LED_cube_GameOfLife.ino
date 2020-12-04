#include "xyz.h"

#define CUBESIZE 4
#define PLANESIZE CUBESIZE*CUBESIZE
#define REFRESH 100

int LEDPin[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, LED_BUILTIN, 14, 15};
int LEDPinCount = 16;
int PlanePin[4] = {16, 17, 18, 19};
int PlanePinCount  = 4;

boolean currentState[4][16] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
boolean previousState[4][16] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

xyz neighbours[] = {{0,-1},{0,1},{4,0},{1,0},{-4,0},{-1,0}}; // use this to find possible neighbours

void setup()
{
  int pin; // loop counter
  // set up LED pins as output (active HIGH)
  for (pin=0; pin<PLANESIZE; pin++) {
    pinMode( LEDPin[pin], OUTPUT );
  }
  // set up plane pins as outputs (active LOW)
  for (pin=0; pin<CUBESIZE; pin++) {
    pinMode( PlanePin[pin], OUTPUT );
  }
  
  //Serial.begin(9600);
  setupState(); // randomize the state

}

void loop(){
  drawState();
  memcpy(previousState,currentState,64);
  progressGame();
}

/*this calucaltes the next Game of Life gamestate according the rules below
*1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
*2. Any live cell with two or three live neighbours lives on to the next generation.
*3. Any live cell with more than three live neighbours dies, as if by overcrowding.
*4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
void progressGame(){
  for(int y=0;y<4;y++){
    for(int xz=0;xz<16;xz++){
      xyz thisLED = {xz,y};
      
      
      int numNeighbours = howManyNeighbours(thisLED);
      
     
      
      if(numNeighbours < 2 && currentState[y][xz]){ //underpopulation
        currentState[y][xz] = false;
     
      }
      else if(numNeighbours > 3 && currentState[y][xz]){ // overcrowding
        currentState[y][xz] = false;
        
      }
      else if(numNeighbours == 3 && !(currentState[y][xz])){ // reproduction
        currentState[y][xz] = true;
        
      }
      else if(currentState[y][xz]){
        
      }
      else{
      
      } 
    }
  }

}
// core to the Game Of Life, this works out exactly what it says using previously written valid function
int howManyNeighbours(xyz led){
  
  
  
  int numNeighbours = 0;
  for(int e = 0;e<6;e++){ //iterate each possible neighbour
    
    // is it out of bounds? and if not, is on?
    xyz neighbourDirection = neighbours[e];
    
    if(valid(led,neighbourDirection)){
      xyz neighbour = move(led,neighbourDirection);
      if(previousState[neighbour.y][neighbour.xz]){
        numNeighbours++;
      }
    }
  }
  return numNeighbours;
}

void setupState(){
  randomSeed(analogRead(20));
  for(int y=0;y<4;y++){
     for(int xz=0;xz<16;xz++){
       currentState[y][xz] = (int)random(2); //initiate every LED to random state
     }   
  }
}

void drawState(){
  long startTime = millis();
  boolean draw = true;
  while(draw){
    for(int y=0;y<4;y++){
      digitalWrite(PlanePin[y],LOW); // turn on this plane
      for(int xz=0;xz<16;xz++){
        digitalWrite(LEDPin[xz],currentState[y][xz]); // set it to wahtever the state says it should be
      }
      delay(2);
      pinsOff();
      digitalWrite(PlanePin[y],HIGH); //turn off this plane before moving to the next one   
    }
    if(millis() > startTime + REFRESH){
      draw = false;
    }
  }
}



void pinsOff(){
  for(int thisPin = 0; thisPin < PlanePinCount; thisPin++){
        digitalWrite(LEDPin[thisPin],LOW);
    }
}

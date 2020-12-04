// Jonathan Holmes (crait)
// October 24th, 2016
// Number Guessing Game

#include <Arduboy.h>

Arduboy arduboy;

int playerwin = 0;
int attempts = 0;
int guessednumber = 0;
int randomnumber = 0;
int upbuffer = 0;
int downbuffer = 0;
int abuffer = 0;
int lastguess = 0;

void setup()
{
  arduboy.begin();
  arduboy.clear();
  srand(7 / 8);
  randomnumber = 1 + rand() % 100;
}

void loop()
{
  arduboy.clear();
  if (playerwin == 0)
  {
    // Ask the player for a number and play the game
    if (attempts == 7)
    {
      // Game Over screen
      arduboy.setCursor(0, 0);
      arduboy.print(F("You lost!\nCorrect Number: "));
      arduboy.print(randomnumber);
      //if (arduboy.pressed(A_BUTTON) == true and abuffer == 0)
      if (arduboy.pressed(A_BUTTON) == true && abuffer == 0)
      {
        abuffer = 1;
        randomnumber = 1 + rand() % 100;
        attempts = playerwin = 0;
      }
    }
    else
    {
      // Player has more attempts
      //if (arduboy.pressed(UP_BUTTON) == true and upbuffer == 0)
      if (arduboy.pressed(UP_BUTTON) == true && upbuffer == 0)
      {
        upbuffer = 1;
        //guessednumber = guessednumber + 1;
        guessednumber++;
      }
      //if (arduboy.pressed(DOWN_BUTTON) == true and downbuffer == 0)
      if (arduboy.pressed(DOWN_BUTTON) == true && downbuffer == 0)
      {
        downbuffer = 1;
        //guessednumber = guessednumber - 1;
        guessednumber--;
      }
      //if (arduboy.pressed(A_BUTTON) == true and abuffer == 0)
      if (arduboy.pressed(A_BUTTON) == true && abuffer == 0)
      {
        abuffer = 1;
        if (guessednumber == randomnumber)
        {
          playerwin = 1;
        }
        else
        {
          //attempts = attempts + 1;
          attempts++;
          lastguess = guessednumber;
        }
      }
      arduboy.setCursor(0, 0);
      arduboy.print(F("Attempt: "));
      arduboy.print(attempts);
      arduboy.print(F("\nNumber to guess: "));
      arduboy.print(guessednumber);
      arduboy.print(F("\n"));
      if (attempts == 0)
      {
        arduboy.print(F("Good luck!"));
      }
      else
      {
        arduboy.print(lastguess);
        if (lastguess > randomnumber) arduboy.print(F(" is too high!"));
        if (lastguess < randomnumber) arduboy.print(F(" is too low!"));
      }
    }
  }
  else
  {
    // Tell the player that they won!
    arduboy.setCursor(0, 0);
    arduboy.print(F("You won!\nCorrect Number: "));
    arduboy.print(randomnumber);
    if (arduboy.pressed(A_BUTTON) == true && abuffer == 0)
    {
      abuffer = 1;
      randomnumber = 1 + rand() % 100;
      attempts = playerwin = 0;
    }
  }
  if (arduboy.notPressed(A_BUTTON) == true) abuffer = 0;
  if (arduboy.notPressed(DOWN_BUTTON) == true) downbuffer = 0;
  if (arduboy.notPressed(UP_BUTTON) == true) upbuffer = 0;
  arduboy.display();
}


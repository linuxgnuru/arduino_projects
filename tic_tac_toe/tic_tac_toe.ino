int computerArray[10] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10
};

typedef enum {
  OPEN,
  CLOSED_C,
  CLOSED_P
} BoxState;

int turn = 0;

int switchArray[10] = {
  11, 12, LED_BUILTIN, 14, 15, 16, 17, 18, 19
};

int switchArray2[10] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10
};

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  for (int i = 0; i < 9; i += 1)
  {
    pinMode(switchArray[i], INPUT);
    pinMode(switchArray2[i], OUTPUT);
  }
}

BoxState box1 = OPEN;
BoxState box2 = OPEN;
BoxState box3 = OPEN;
BoxState box4 = OPEN;
BoxState box5 = OPEN;
BoxState box6 = OPEN;
BoxState box7 = OPEN;
BoxState box8 = OPEN;
BoxState box9 = OPEN;

int game = 1;
int Array1[9] = {
  13, 19, 16, 13, 12, 11, 13, 11
};
int Array2[9] = {
  12, 14, 17, 19, 14, 15, 14, 14
};
int Array3[9] = {
  11, 15, 18, 16, 17, 18, 18, 16
};

void checkGame()
{
  int q;
  for (q = 0; q < 8; q += 1)
  {
    if (digitalRead(Array1[q]) == HIGH && digitalRead(Array2[q]) && digitalRead(Array3[q]))
    {
      game = 2;
    }
  }
}

void checkComputerWin()
{
  if (turn % 2 == 1)
  {
    if (box1 == CLOSED_C && box2 == CLOSED_C && box3 == OPEN)
    {
      box3 = CLOSED_C;
      digitalWrite(7, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box1 == CLOSED_C && box3 == CLOSED_C && box2 == OPEN)
    {
      box2 = CLOSED_C;
      digitalWrite(8, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box3 == CLOSED_C && box2 == CLOSED_C && box1 == OPEN)
    {
      box1 = CLOSED_C;
      digitalWrite(9, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box4 == CLOSED_C && box5 == CLOSED_C && box6 == OPEN)
    {
      box6 = CLOSED_C;
      digitalWrite(4, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box5 == CLOSED_C && box6 == CLOSED_C && box4 == OPEN)
    {
      box4 = CLOSED_C;
      digitalWrite(6, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box4 == CLOSED_C && box6 == CLOSED_C && box5 == OPEN)
    {
      box5 = CLOSED_C;
      digitalWrite(5, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box7 == CLOSED_C && box8 == CLOSED_C && box9 == OPEN)
    {
      box9 = CLOSED_C;
      digitalWrite(10, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box8 == CLOSED_C && box9 == CLOSED_C && box7 == OPEN)
    {
      box7 = CLOSED_C;
      digitalWrite(3, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box7 == CLOSED_C && box9 == CLOSED_C && box8 == OPEN)
    {
      box8 = CLOSED_C;
      digitalWrite(2, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box1 == CLOSED_C && box4 == CLOSED_C && box7 == OPEN)
    {
      box7 = CLOSED_C;
      digitalWrite(3, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box1 == CLOSED_C && box7 == CLOSED_C && box4 == OPEN)
    {
      box4 = CLOSED_C;
      digitalWrite(6, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box4 == CLOSED_C && box7 == CLOSED_C && box1 == OPEN)
    {
      box1 = CLOSED_C;
      digitalWrite(9, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box5 == CLOSED_C && box2 == CLOSED_C && box8 == OPEN)
    {
      box8 = CLOSED_C;
      digitalWrite(2, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box8 == CLOSED_C && box2 == CLOSED_C && box5 == OPEN)
    {
      box5 = CLOSED_C;
      digitalWrite(5, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box5 == CLOSED_C && box8 == CLOSED_C && box2 == OPEN)
    {
      box2 = CLOSED_C;
      digitalWrite(8, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box3 == CLOSED_C && box6 == CLOSED_C && box9 == OPEN)
    {
      box9 = CLOSED_C;
      digitalWrite(10, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box6 == CLOSED_C && box9 == CLOSED_C && box3 == OPEN)
    {
      box3 = CLOSED_C;
      digitalWrite(7, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box3 == CLOSED_C && box9 == CLOSED_C && box6 == OPEN)
    {
      box6 = CLOSED_C;
      digitalWrite(4, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box1 == CLOSED_C && box5 == CLOSED_C && box9 == OPEN)
    {
      box9 = CLOSED_C;
      digitalWrite(10, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box5 == CLOSED_C && box9 == CLOSED_C && box1 == OPEN)
    {
      box1 = CLOSED_C;
      digitalWrite(9, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box9 == CLOSED_C && box1 == CLOSED_C && box5 == OPEN)
    {
      box5 = CLOSED_C;
      digitalWrite(5, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box3 == CLOSED_C && box5 == CLOSED_C && box7 == OPEN)
    {
      box7 = CLOSED_C;
      digitalWrite(3, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box7 == CLOSED_C && box5 == CLOSED_C && box3 == OPEN)
    {
      box3 = CLOSED_C;
      digitalWrite(7, HIGH);
      game = 0;
      turn += 1;
    }
    else if (box3 == CLOSED_C && box7 == CLOSED_C && box5 == OPEN)
    {
      box5 = CLOSED_C;
      digitalWrite(5, HIGH);
      game = 0;
      turn += 1;
    }
  }
}

void checkPlayerWin()
{
  if (box1 == CLOSED_P && box2 == CLOSED_P && box3 == OPEN && turn % 2 == 1)
  {
    box3 = CLOSED_C;
    digitalWrite(7, HIGH);
    turn += 1;
  }
  if (box1 == CLOSED_P && box3 == CLOSED_P && box2 == OPEN && turn % 2 == 1)
  {
    box2 = CLOSED_C;
    digitalWrite(8, HIGH);
    turn += 1;
  }
  if (box3 == CLOSED_P && box2 == CLOSED_P && box1 == OPEN && turn % 2 == 1)
  {
    box1 = CLOSED_C;
    digitalWrite(9, HIGH);
    turn += 1;
  }
  if (box4 == CLOSED_P && box5 == CLOSED_P && box6 == OPEN && turn % 2 == 1)
  {
    box6 = CLOSED_C;
    digitalWrite(4, HIGH);
    turn += 1;
  }
  if (box5 == CLOSED_P && box6 == CLOSED_P && box4 == OPEN && turn % 2 == 1)
  {
    box4 = CLOSED_C;
    digitalWrite(6, HIGH);
    turn += 1;
  }
  if (box4 == CLOSED_P && box6 == CLOSED_P && box5 == OPEN && turn % 2 == 1)
  {
    box5 = CLOSED_C;
    digitalWrite(5, HIGH);
    turn += 1;
  }
  if (box7 == CLOSED_P && box8 == CLOSED_P && box9 == OPEN && turn % 2 == 1)
  {
    box9 = CLOSED_C;
    digitalWrite(10, HIGH);
    turn += 1;
  }
  if (box8 == CLOSED_P && box9 == CLOSED_P && box7 == OPEN && turn % 2 == 1)
  {
    box7 = CLOSED_C;
    digitalWrite(3, HIGH);
    turn += 1;
  }
  if (box7 == CLOSED_P && box9 == CLOSED_P && box8 == OPEN && turn % 2 == 1)
  {
    box8 = CLOSED_C;
    digitalWrite(2, HIGH);
    turn += 1;
  }
  if (box1 == CLOSED_P && box4 == CLOSED_P && box7 == OPEN && turn % 2 == 1)
  {
    box7 = CLOSED_C;
    digitalWrite(3, HIGH);
    turn += 1;
  }
  if (box1 == CLOSED_P && box7 == CLOSED_P && box4 == OPEN && turn % 2 == 1)
  {
    box4 = CLOSED_C;
    digitalWrite(6, HIGH);
    turn += 1;
  }
  if (box4 == CLOSED_P && box7 == CLOSED_P && box1 == OPEN && turn % 2 == 1)
  {
    box1 = CLOSED_C;
    digitalWrite(9, HIGH);
    turn += 1;
  }
  if (box5 == CLOSED_P && box2 == CLOSED_P && box8 == OPEN && turn % 2 == 1)
  {
    box8 = CLOSED_C;
    digitalWrite(2, HIGH);
    turn += 1;
  }
  if (box8 == CLOSED_P && box2 == CLOSED_P && box4 == OPEN && turn % 2 == 1)
  {
    box5 = CLOSED_C;
    digitalWrite(5, HIGH);
    turn += 1;
  }
  if (box5 == CLOSED_P && box8 == CLOSED_P && box2 == OPEN && turn % 2 == 1)
  {
    box2 = CLOSED_C;
    digitalWrite(8, HIGH);
    turn += 1;
  }
  if (box3 == CLOSED_P && box6 == CLOSED_P && box9 == OPEN && turn % 2 == 1)
  {
    box9 = CLOSED_C;
    digitalWrite(10, HIGH);
    turn += 1;
  }
  if (box6 == CLOSED_P && box9 == CLOSED_P && box3 == OPEN && turn % 2 == 1)
  {
    box3 = CLOSED_C;
    digitalWrite(7, HIGH);
    turn += 1;
  }
  if (box3 == CLOSED_P && box9 == CLOSED_P && box6 == OPEN && turn % 2 == 1)
  {
    box6 = CLOSED_C;
    digitalWrite(4, HIGH);
    turn += 1;
  }
  if (box1 == CLOSED_P && box5 == CLOSED_P && box9 == OPEN && turn % 2 == 1)
  {
    box9 = CLOSED_C;
    digitalWrite(10, HIGH);
    turn += 1;
  }
  if (box5 == CLOSED_P && box9 == CLOSED_P && box1 == OPEN && turn % 2 == 1)
  {
    box1 = CLOSED_C;
    digitalWrite(9, HIGH);
    turn += 1;
  }
  if (box9 == CLOSED_P && box1 == CLOSED_P && box5 == OPEN && turn % 2 == 1)
  {
    box5 = CLOSED_C;
    digitalWrite(5, HIGH);
    turn += 1;
  }
  if (box3 == CLOSED_P && box5 == CLOSED_P && box7 == OPEN && turn % 2 == 1)
  {
    box7 = CLOSED_C;
    digitalWrite(3, HIGH);
    turn += 1;
  }
  if (box7 == CLOSED_P && box5 == CLOSED_P && box3 == OPEN && turn % 2 == 1)
  {
    box3 = CLOSED_C;
    digitalWrite(7, HIGH);
    turn += 1;
  }
  if (box3 == CLOSED_P && box7 == CLOSED_P && box5 == OPEN && turn % 2 == 1)
  {
    box5 = CLOSED_C;
    digitalWrite(5, HIGH);
    turn += 1;
  }

}

void playRandom()
{
  int ran = random(0, 90);
  if (ran <= 10 && box1 == OPEN && turn % 2 == 1)
  {
    box1 = CLOSED_C;
    turn += 1;
    digitalWrite(9, HIGH);
  }
  if (ran <= 20 && ran > 10 && box2 == OPEN && turn % 2 == 1)
  {
    box2 = CLOSED_C;
    turn += 1;
    digitalWrite(8, HIGH);
  }
  if (ran <= 30 && ran > 20 && box3 == OPEN && turn % 2 == 1)
  {
    box3 = CLOSED_C;
    turn += 1;
    digitalWrite(7, HIGH);
  }
  if (ran <= 40 && ran > 30 && box4 == OPEN && turn % 2 == 1)
  {
    box4 = CLOSED_C;
    turn += 1;
    digitalWrite(6, HIGH);
  }
  if (ran <= 50 && ran > 40 && box5 == OPEN && turn % 2 == 1)
  {
    box5 = CLOSED_C;
    turn += 1;
    digitalWrite(5, HIGH);
  }
  if (ran <= 60 && ran > 50 && box6 == OPEN && turn % 2 == 1)
  {
    box6 = CLOSED_C;
    turn += 1;
    digitalWrite(4, HIGH);
  }
  if (ran <= 70 && ran > 60 && box7 == OPEN && turn % 2 == 1)
  {
    box7 = CLOSED_C;
    turn += 1;
    digitalWrite(3, HIGH);
  }
  if (ran <= 80 && ran > 70 && box8 == OPEN && turn % 2 == 1)
  {
    box8 = CLOSED_C;
    turn += 1;
    digitalWrite(2, HIGH);
  }
  if (ran <= 90 && ran > 80 && box9 == OPEN && turn % 2 == 1)
  {
    box9 = CLOSED_C;
    turn += 1;
    digitalWrite(10, HIGH);
  }
}

void loop()
{
  if (turn % 2 == 0) {
    if (digitalRead(19) == HIGH && box4 == OPEN)
    {
      box4 = CLOSED_P;
      turn += 1;
    }
    if (digitalRead(11) == HIGH && box3 == OPEN)
    {
      box3 = CLOSED_P;
      turn += 1;
    }
    if (digitalRead(12) == HIGH && box2 == OPEN)
    {
      box2 = CLOSED_P;
      turn += 1;
    }
    if (digitalRead(13) == HIGH && box1 == OPEN)
    {
      box1 = CLOSED_P;
      turn += 1;
    }
    if (digitalRead(14) == HIGH && box5 == OPEN)
    {
      box5 = CLOSED_P;
      turn += 1;
    }
    if (digitalRead(15) == HIGH && box6 == OPEN)
    {
      box6 = CLOSED_P;
      turn += 1;
    }
    if (digitalRead(16) == HIGH && box7 == OPEN)
    {
      box7 = CLOSED_P;
      turn += 1;
    }
    if (digitalRead(17) == HIGH && box8 == OPEN)
    {
      box8 = CLOSED_P;
      turn += 1;
    }
    if (digitalRead(18) == HIGH && box9 == OPEN)
    {
      box9 = CLOSED_P;
      turn += 1;
    }
  }
  if (turn % 2 == 1)
  {
    checkComputerWin();
    checkPlayerWin();
    playRandom();
  }
  checkGame();
  if (game == 0 || game == 2)
  {
    delay(2000);
    int t;
    for (t = 0; t < 9; t += 1)
    {
      digitalWrite(computerArray[t], HIGH);
    }
  }
}


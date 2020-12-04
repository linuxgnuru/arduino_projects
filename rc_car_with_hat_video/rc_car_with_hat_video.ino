typedef enum {
  NOCHANGE_  = -1,
  RIGHT_,
  LEFT_,
  MIDDLE_,
  FAR_RIGHT_,
  FAR_LEFT_
} dir_enum;

unsigned long lm = 0;

const byte anaPin = A1;
const byte E1 = 5;
const byte I1 = 2;
const byte I2 = 3;

const byte buttonPinLeft = 8;
int buttonStateLeft;
int lastButtonStateLeft = HIGH;
unsigned long lastDebounceTimeLeft = 0;

const byte buttonPinRight = 9;
int buttonStateRight;
int lastButtonStateRight = HIGH;
unsigned long lastDebounceTimeRight = 0;

unsigned long debounceDelay = 50;

void setup() {
  pinMode(E1, OUTPUT);
  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);
  // make the other half of the h bridge grounded as we won't be using it
  pinMode(4, OUTPUT); digitalWrite(4, LOW);
  pinMode(6, OUTPUT); digitalWrite(6, LOW);
  pinMode(7, OUTPUT); digitalWrite(7, LOW);

  pinMode(buttonPinLeft, INPUT_PULLUP);
  pinMode(buttonPinRight, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  int cd = getDir();
  while (cd != MIDDLE_) {
    if (cd == RIGHT_ || cd == FAR_RIGHT_) {
      runServo(LEFT_);
    } else if (cd == LEFT_ || cd == FAR_LEFT_) {
      runServo(RIGHT_);
    }
    cd = getDir();
  }
}

void loop() {
  int readingLeft = digitalRead(buttonPinLeft);
  if (readingLeft != lastButtonStateLeft) {
    lastDebounceTimeLeft = millis();
  }
  if ((millis() - lastDebounceTimeLeft) > debounceDelay) {
    if (readingLeft != buttonStateLeft) {
      buttonStateLeft = readingLeft;
      if (buttonStateLeft == LOW) {
        runServo(LEFT_);
      }
    }
  }
  lastButtonStateLeft = readingLeft;

  int readingRight = digitalRead(buttonPinRight);
  if (readingRight != lastButtonStateRight) {
    lastDebounceTimeRight = millis();
  }
  if ((millis() - lastDebounceTimeRight) > debounceDelay) {
    if (readingRight != buttonStateRight) {
      buttonStateRight = readingRight;
      if (buttonStateRight == LOW) {
        runServo(RIGHT_);
      }
    }
  }
  lastButtonStateRight = readingRight;
}

void runServo(int dir) {
  int c = getDir();

  switch (c) {
    case NOCHANGE_:
      break;
    case RIGHT_: case FAR_RIGHT_:
      if (dir == LEFT_ || dir == FAR_LEFT_) { // left
        Serial.println(F(" -- [right] Moving Left --"));
        c = getDir();
        while (c != MIDDLE_) {
          digitalWrite(I1, HIGH);
          digitalWrite(I2, LOW);
          analogWrite(E1, 255);
          c = getDir();
        }
      }
      break;
    case MIDDLE_:
      if (dir == LEFT_ || dir == FAR_LEFT_) { // left
        Serial.println(F(" -- [middle] Moving Left --"));
        c = getDir();
        while (c != FAR_LEFT_) {
          digitalWrite(I1, HIGH);
          digitalWrite(I2, LOW);
          analogWrite(E1, 255);
          c = getDir();
        }
      }
      if (dir == RIGHT_ || dir == FAR_RIGHT_) { // right
        Serial.println(F(" -- [middle] Moving Right --"));
        c = getDir();
        while (c != FAR_RIGHT_) {
          digitalWrite(I1, LOW);
          digitalWrite(I2, HIGH);
          analogWrite(E1, 255);
          c = getDir();
        }
      }
      break;
    case LEFT_: case FAR_LEFT_:
      if (dir == RIGHT_ || dir == FAR_RIGHT_) { // right
        Serial.println(F(" -- [left] Moving Right --"));
        c = getDir();
        while (c != MIDDLE_) {
          digitalWrite(I1, LOW);
          digitalWrite(I2, HIGH);
          analogWrite(E1, 255);
          c = getDir();
        }
      }
      break;
    default:
      Serial.println(F(" -- ERROR -- "));
      break;
  }
  digitalWrite(I1, LOW);
  digitalWrite(I2, LOW);
  analogWrite(E1, 0);
}

int getDir() {
  int r = NOCHANGE_;
  int raw;
  float R2 = 0;
  boolean cont = false;

  // Get an average
  for (int i = 0; i < 10; i++) {
    raw = analogRead(anaPin);
    if (raw < 1020) {
      R2 += (10 / ((5 / ((5.0 / 1023.0) * raw)) - 1));
      cont = true;
    }
  }
  if (cont) {
    R2 /= 10;
    if (R2 > 3.0) { // left
      r = (R2 >= 3.57 ? FAR_LEFT_ : LEFT_);
    } else if (R2 > 2.5 && R2 < 2.9) { // middle
      r = MIDDLE_;
    } else if (R2 < 2.5) { // right
      r = (R2 <= 1.37 ? FAR_RIGHT_ : RIGHT_);
    }
  }
  return r;
}


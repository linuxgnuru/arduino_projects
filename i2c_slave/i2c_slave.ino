#include <Wire.h>

#define SLAVE_ADDRESS 0x05

const int waitDelay = 1000;
unsigned long lastMillis = 0;
unsigned long lastMillisBreak = 0;

// ranges for left and right:
//const float vFarLeftMax = 4.0;

volatile int command;
volatile int data;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // because we are only using half of the h bridge; ground the other half
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;
    digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
  }
}

void receiveData(int byteCount)
{
  while (Wire.available())
  {
    command = Wire.read();
    switch (command)
    {
      case 0:
        data = 1;
        break;
      default:
        data = 2;
        break;
    }
  }
}

void sendData()
{
  Wire.write(data);
}


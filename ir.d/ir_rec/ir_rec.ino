//#include <Wire.h>
#include <IRremote.h>

//#define SLAVE_ADDRESS 0x04

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
int return_val = 11;

void setup()
{
//  Wire.begin(SLAVE_ADDRESS);
//  Wire.onReceive(receiveData);
//  Wire.onRequest(sendData);
    Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
void loop()
{
  if (irrecv.decode(&results))
  {
    return_val = results.value;
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
/*
void receiveData(int byteCount)
{
  while (Wire.available())
  {
    return_val = Wire.read();
    switch (return_val)
    {
        case 1:
          return_val = decimal;
          break;
        case 2:
          ForC = (ForC == CEL ? FAR : CEL);
          return_val = 70;
          break;
        case 3: // internal temp of Arduino
          return_val = (int)GetTemp();
          break;
        default:
          break;
    }
  }
}

void sendData()
{
  Wire.write(return_val);
}
*/

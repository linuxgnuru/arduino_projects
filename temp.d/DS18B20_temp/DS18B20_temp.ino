#include <OneWire.h>

OneWire ds(10); // pin 10

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  int Temp;
  if (!ds.search(addr))
  {
    ds.reset_search();
    return;
  }
  /*
  Serial.print("R=");
  for (i = 0; i < 8; i++)
  {
    Serial.print(addr[i], HEX);
    Serial.print("");
  }
  */
  if (OneWire::crc8(addr, 7) != addr[7])
  {
    Serial.print("CRC is not valid!\n");
    return;
  }
  if (addr[0] != 0x28)
  {
    Serial.print("Device is not a DS1820 family device.\n");
    return;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  delay(1000);
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  /*
  Serial.println();
  Serial.print("P=");
  Serial.print(present, HEX);
  Serial.print("");
  */
  for (i = 0; i < 9; i++)
  {
    data[i] = ds.read();
    //Serial.print(data[i], HEX);
  }
  Temp = (data[1]<<8) + data[0];
  Temp = Temp>>4;
  Temp = Temp * 1.8 + 32; // convert C to F
  //Serial.println();
  Serial.print("");
  Serial.println(Temp);
//  Serial.print("");
/*
  Serial.print("CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();
  */
}

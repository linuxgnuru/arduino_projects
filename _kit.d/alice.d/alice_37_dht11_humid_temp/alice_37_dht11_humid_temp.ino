//KY015 DHT11 Temperature and humidity sensor 
int DHpin = 8;
int dhVccPin = 9;
int dhGndPin = 10;
byte dat[5];

byte read_data()
{
  byte data;
  for (int i = 0; i < 8; i ++)
  {
    if (digitalRead(DHpin) == LOW)
    {
      // wait for 50us
      while (digitalRead(DHpin) == LOW)
        ;
      // determine the duration of the high level to determine the data is '0 'or '1'
      delayMicroseconds(30);
      // high front and low in the post
      if (digitalRead(DHpin) == HIGH)
        data |= (1 << (7-i));
      // data '1 ', wait for the next one receiver
      while (digitalRead (DHpin) == HIGH)
        ;
     }
  }
  return data;
}
 
void start_test()
{
  // bus down, send start signal
  digitalWrite(DHpin, LOW);
  // delay greater than 18ms, so DHT11 start signal can be detected
  delay(30);
  digitalWrite(DHpin, HIGH);
  // Wait for DHT11 response
  delayMicroseconds(40);
  pinMode(DHpin, INPUT);
  while (digitalRead(DHpin) == HIGH)
    ;
  // DHT11 response, pulled the bus 80us
  delayMicroseconds(80);
  if (digitalRead(DHpin) == LOW)
    ;
  // DHT11 80us after the bus pulled to start sending data
  delayMicroseconds(80);
  // receive temperature and humidity data, the parity bit is not considered
  for (int i = 0; i < 4; i ++)
    dat[i] = read_data();
  pinMode(DHpin, OUTPUT);
  // send data once after releasing the bus, wait for the host to open the next Start signal
  digitalWrite(DHpin, HIGH);
}
 
void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DHpin, OUTPUT);
  pinMode(dhVccPin, OUTPUT);
  pinMode(dhGndPin, OUTPUT);
  digitalWrite(dhGndPin, LOW);
  digitalWrite(dhVccPin, HIGH);
}
 
void loop()
{
  start_test();
  Serial.print("Current humdity =");
  // display the humidity-bit integer;
  Serial.print(dat[0], DEC);
  Serial.print('.');
  // display the humidity decimal places;
  Serial.print(dat[1], DEC);
  Serial.println('%');
  Serial.print("Current temperature =");
  // display the temperature of integer bits;
  Serial.print(dat[2], DEC);
  Serial.print('.');
  // display the temperature of decimal places;
  Serial.print(dat[3], DEC);
  Serial.println('C');
  delay(700);
}

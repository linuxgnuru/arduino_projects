/*****************************************/
const int ledPinFront = 12;//the led attach to
const int ledPinTop = 11;
const int ledPinHall = 10;
const int senPinFront = 2;
const int senPinTop = 3;

//const int senPinHall = 4;
//const int anaPinHall = A0;

void setup()
{ 
  pinMode(ledPinFront,OUTPUT);//initialize the ledPin as an output
  pinMode(ledPinTop,OUTPUT);//initialize the ledPin as an output
//  pinMode(ledPinHall,OUTPUT);//initialize the ledPin as an output
  pinMode(senPinFront,INPUT);
  pinMode(senPinTop, INPUT);
//  pinMode(senPinHall, INPUT);
//  Serial.begin(9600);

//  digitalWrite(2, HIGH);
//  digitalWrite(
} 
/******************************************/
void loop() 
{  
  int digitalValFront = digitalRead(senPinFront);
  int digitalValTop = digitalRead(senPinTop);
//  int digitalValHall = digitalRead(senPinHall);
  if(HIGH == digitalValFront)    digitalWrite(ledPinFront,LOW);//turn the led off
  else    digitalWrite(ledPinFront,HIGH);//turn the led on 
  if(HIGH == digitalValTop)    digitalWrite(ledPinTop,LOW);//turn the led off
  else    digitalWrite(ledPinTop,HIGH);//turn the led on 
/*
  if(HIGH == digitalValHall)    digitalWrite(ledPinTop,LOW);//turn the led off
  else    digitalWrite(ledPinHall,HIGH);//turn the led on 

  long a = analogRead(anaPinHall);
  Serial.print("Hall: ");
  Serial.println(a);
  delay(200);
  */
}
/**********************************************/


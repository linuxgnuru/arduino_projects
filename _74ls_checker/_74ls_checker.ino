/* 
 Design a circuit and write a test program, using the Arduino Nano,
 that will test two input Transistor-Transistor Logic (TTL) gates
 including AND, OR, NAND, and XOR.
 By: Zoran M
 */
int gatevalue; //Declare all of the variables
int inputPin = 4;
int Output1Pin = 2;
int Output2Pin = 3;
int led = LED_BUILTIN;
int led2 = 12;
int led3 = 11;
int led4 = 10;
int led5 = 9;

//Begin declaring what pin is what
void setup()
{
  Serial.begin(9600);
  pinMode (Output1Pin, OUTPUT);
  pinMode (Output2Pin, OUTPUT);
  pinMode (inputPin, INPUT);
  pinMode(led, OUTPUT); 
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  gatevalue = 0;
}

void loop()
{
  gatevalue = gatevalue + (check_Gate(false, false)*8); //When an input is HIGH it will add a value to the gatevalue
  gatevalue = gatevalue + (check_Gate(false, true)*4);  //and each gate will have a different gatevalue.
  gatevalue = gatevalue + (check_Gate(true, false)*2);
  gatevalue = gatevalue + (check_Gate(true, true)*1);

  switch (gatevalue)
  {
  case 1:
    Serial.println("The gate is an AND gate."); //When gatevalue is 1 it is an AND gate
    digitalWrite(led, LOW); //if gate is in, light is off
    digitalWrite(led2, LOW); // AND gate truth table outputs are 0, 0, 0, 1 which is how the LED’s are set up
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, HIGH);
    break;
  case 6:
    Serial.println("The gate is an XOR gate.");
    digitalWrite(led, LOW); //if gate is in, light is off
    digitalWrite(led2, LOW);//Truth table values
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, LOW);
    break;
  case 7:
    Serial.println("The gate is an OR gate.");
    digitalWrite(led, LOW); //if gate is in, light is off
    digitalWrite(led2, LOW); //Truth table values
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    break;
  case 14:
    Serial.println("The gate is an NAND gate.");
    digitalWrite(led, LOW); //if gate is in, light is off
    digitalWrite(led2, HIGH); //Truth table values
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, LOW);
    break;
  default:
    Serial.println("ERROR: Gate Not Present.");
    digitalWrite(led, HIGH); //if gate is not in, light is on
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
  }
  gatevalue = 0;
  delay(1000);
}

int check_Gate(int output1, int output2)
{
  int x;
  digitalWrite(Output1Pin, output1);
  digitalWrite(Output2Pin, output2);
  delay(5); // Make sure the signal has time to propogate through the gate.
  x = digitalRead(inputPin);

  return x;
}


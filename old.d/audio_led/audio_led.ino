int DO = 2; //Pin for Digital Output - DO
int DA = A0; // Pin for Analog Output - AO
int threshold = 532; //Set minimum threshold for LED lit
int sensorvalue = 0;

void setup() {
  //Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  sensorvalue = analogRead(DA);  //Read the analog value
  //Serial.print("Analog: ");
  //Serial.print(sensorvalue);  //Print the analog value
  //Serial.print("  ");
  //Serial.print("Digital: ");
  //Serial.println(digitalRead(DO));  //Print the digital value

  if (sensorvalue >= threshold) { //Compare analog value with threshold
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);

  }
  else {
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
  }
}

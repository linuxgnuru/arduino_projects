int sensorPin = A0;

double alpha = 0.75;
int period = 20;
double change = 0.0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  static double oldValue = 0;
  static double oldChange = 0;
  int rawValue = analogRead(sensorPin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;
  
  Serial.print(rawValue);
  Serial.print(",");
  Serial.println(value);
  
  oldValue = value;
  delay(period);
}

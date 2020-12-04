#define NUM_SAMPLES 10

const int motorBatPin = A2;

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  checkBat();
}

void checkBat()
{
  float retVal = 0.0;
  while (sample_count < NUM_SAMPLES)
  {
    sum += analogRead(motorBatPin);
    sample_count++;
    delay(10);
  }
  voltage = ((float)sum / (float)NUM_SAMPLES * 5.09) / 1024.0;
  retVal = voltage * 11.069;
  if (retVal < 0.09)
    retVal = 0.0;
  sample_count = 0;
  sum = 0;
  digitalWrite(LED_BUILTIN, (retVal < 4 && retVal != 0));
}


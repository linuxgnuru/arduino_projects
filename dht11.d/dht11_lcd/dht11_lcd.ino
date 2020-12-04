#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 9     // what pin we're connected to
#define DHTTYPE DHT11

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const byte inputPin = 8;

unsigned long lastM = 0;
boolean cf = false;
#define DL 500

byte degree[8] = {
  0b01100,
  0b10010,
  0b10010,
  0b01100,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup()
{
  //Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.createChar(2, degree);
  pinMode(inputPin, INPUT_PULLUP);
}

void loop()
{
  int h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  unsigned long currentM = millis();

  cf = digitalRead(inputPin);
  if (currentM - lastM >= DL)
  {
    lastM = currentM;
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f))
    {
      lcd.setCursor(0, 0);
      //Serial.println("Failed to read from DHT sensor!");
      //         123456789012345
      lcd.print("Failed to read");
      lcd.setCursor(0, 1);
      lcd.print("from DHT sensor!");
      return;
    }
    lcd.setCursor(0, 0);
    // Compute heat index
    // Must send in temp in Fahrenheit!
    float hi = dht.computeHeatIndex(f, h);
    lcd.print("heat idx: ");
    lcd.print(hi);
    //Serial.print("Humidity: ");
    //Serial.println(h);
    //lcd.print(" hum: ");
    //lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("h ");
    lcd.print(h);
    lcd.print("% t ");
    //lcd.print("temp: ");
    if (cf)
    {
      lcd.print(f);
      lcd.write(2);
      lcd.print("F");
    }
    else
    {
      lcd.print(t);
      lcd.write(2);
      lcd.print("C");
    }
    //Serial.print("Temperature: ");
    //Serial.print(t);
    //delay(500);                        // wait 100ms for next reading
  }
}


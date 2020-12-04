#include <DHT.h>
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(9600);
  dht.begin();
}

void loop() {

  int h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  //Serial.print("Humidity: ");
  Serial.println(h);
  //Serial.print("Temperature: ");
  Serial.print(t);
  delay(500);                        // wait 100ms for next reading


}

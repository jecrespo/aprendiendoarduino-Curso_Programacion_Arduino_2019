#include "DHT.h"

#define DHTPIN_1 11     // Digital pin connected to the DHT sensor
#define DHTPIN_2 13     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht1(DHTPIN_1, DHTTYPE);
DHT dht2(DHTPIN_2, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 test!"));

  dht1.begin();
  dht2.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float h1 = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  float t1 = dht1.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f1 = dht1.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h1) || isnan(t1) || isnan(f1)) {
    Serial.println(F("Failed to read from DHT sensor 1!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif1 = dht1.computeHeatIndex(f1, h1);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic1 = dht1.computeHeatIndex(t1, h1, false);

  Serial.print(F("Humidity 1: "));
  Serial.print(h1);
  Serial.print(F("%  Temperature 1: "));
  Serial.print(t1);
  Serial.print(F("°C "));
  Serial.print(f1);
  Serial.print(F("°F  Heat index 1: "));
  Serial.print(hic1);
  Serial.print(F("°C "));
  Serial.print(hif1);
  Serial.println(F("°F"));

  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float h2 = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  float t2 = dht2.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f2 = dht2.readTemperature(true);

   // Check if any reads failed and exit early (to try again).
  if (isnan(h2) || isnan(t2) || isnan(f2)) {
    Serial.println(F("Failed to read from DHT sensor 2!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif2 = dht2.computeHeatIndex(f2, h2);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic2 = dht2.computeHeatIndex(t2, h2, false);

  Serial.print(F("Humidity 2: "));
  Serial.print(h2);
  Serial.print(F("%  Temperature 2: "));
  Serial.print(t2);
  Serial.print(F("°C "));
  Serial.print(f2);
  Serial.print(F("°F  Heat index 2: "));
  Serial.print(hic2);
  Serial.print(F("°C "));
  Serial.print(hif2);
  Serial.println(F("°F"));
}

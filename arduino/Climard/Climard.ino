#include "DHT.h"

#define DHTPIN 4    // Pin where the DHT11 is connected
#define DHTTYPE DHT11
#define LDRPIN  33   

#define blueLed 12
#define yellowLed 13

DHT dht(DHTPIN, DHTTYPE);

// Threshold values
float tempThreshold = 30.0;  // Temperature threshold
int ldrThreshold = 500;      // LDR threshold

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Initialize LEDs
  pinMode(blueLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  digitalWrite(blueLed, LOW);
  digitalWrite(yellowLed, LOW);
}

void loop() {
  // Reading humidity and temperature
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Reading LDR value
  int ldrValue = analogRead(LDRPIN);

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");

    // Check temperature threshold and control blue LED
    if (temperature > tempThreshold) {
      digitalWrite(blueLed, HIGH); // Turn on blue LED
    } else {
      digitalWrite(blueLed, LOW);  // Turn off blue LED
    }
  }

  // Check LDR threshold and control yellow LED
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  if (ldrValue > ldrThreshold) {
    digitalWrite(yellowLed, HIGH);  // Turn on yellow LED
  } else {
    digitalWrite(yellowLed, LOW);   // Turn off yellow LED
  }

  delay(500); // Wait 2 seconds before next reading
}

#include <Adafruit_Sensor.h>
#include <DHT.h>

// Define pin connections
#define DHTPIN 15        // Pin connected to the DHT11 data pin
#define DHTTYPE DHT11    // DHT 11
#define LDRPIN 34        // Pin connected to the LDR
#define led_blue 22       // Pin connected to the first LED
#define led_yellow 23       // Pin connected to the second LED
#define led_inbuilt 2

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize LED pins
  pinMode(led_blue, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(led_inbuilt, OUTPUT);
}

void loop() {

  // triggering board led
  digitalWrite(led_inbuilt, HIGH);

  // Read temperature and humidity from DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Read value from LDR
  int ldrValue = analogRead(LDRPIN);
  
  // Print sensor values to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, LDR: ");
  Serial.println(ldrValue);

  // Control LEDs based on sensor values
  if (temperature > 37) {
    digitalWrite(led_blue, HIGH); // Turn on LED1 if temperature > 25°C
  } else {
    digitalWrite(led_blue, LOW);
  }

  if (ldrValue > 850) {
    digitalWrite(led_yellow, HIGH); // Turn on LED2 if light level is low
  } else {
    digitalWrite(led_yellow, LOW);
  }

  delay(2000); // Update every 1.0 seconds
}

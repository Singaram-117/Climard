#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT11
#define LDRPIN 34
#define led_blue 22
#define led_yellow 23
#define led_inbuilt 2

const char* ssid = "Bravo";
const char* password = "bravo220";
const char* serverName = "http://192.168.1.100:8000/endpoint/";
const char* thresholdsUrl = "http://192.168.1.100:8000/api/get_thresholds/";

DHT dht(DHTPIN, DHTTYPE);

// Threshold values
float TEMP_THRESHOLD = 37.0;
int LDR_THRESHOLD = 850;

unsigned long lastThresholdCheck = 0;
const unsigned long thresholdCheckInterval = 10000; // 10 seconds

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(led_blue, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(led_inbuilt, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  fetchThresholds();
}

void loop() {
  digitalWrite(led_inbuilt, HIGH);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int ldrValue = analogRead(LDRPIN);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, LDR: ");
  Serial.println(ldrValue);

  if (temperature > TEMP_THRESHOLD) {
    digitalWrite(led_blue, HIGH);
  } else {
    digitalWrite(led_blue, LOW);
  }

  if (ldrValue > LDR_THRESHOLD) {
    digitalWrite(led_yellow, HIGH);
  } else {
    digitalWrite(led_yellow, LOW);
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + ", \"ldr\": " + String(ldrValue) + "}";

    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  // Check thresholds every 10 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - lastThresholdCheck >= thresholdCheckInterval) {
    lastThresholdCheck = currentMillis;
    fetchThresholds();
  }

  delay(1000);
}

void fetchThresholds() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(thresholdsUrl);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Thresholds response: " + response);

      // Use ArduinoJson to parse the response
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);
      float newTempThreshold = doc["temp_threshold"];
      int newLdrThreshold = doc["ldr_threshold"];

      // Update threshold values
      TEMP_THRESHOLD = newTempThreshold;
      LDR_THRESHOLD = newLdrThreshold;

      // Control LEDs based on new thresholds
      updateLEDs();
    } else {
      Serial.print("Error on getting thresholds: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

void updateLEDs() {
  float temperature = dht.readTemperature();
  int ldrValue = analogRead(LDRPIN);

  if (temperature > TEMP_THRESHOLD) {
    digitalWrite(led_blue, HIGH);
  } else {
    digitalWrite(led_blue, LOW);
  }

  if (ldrValue > LDR_THRESHOLD) {
    digitalWrite(led_yellow, HIGH);
  } else {
    digitalWrite(led_yellow, LOW);
  }
}

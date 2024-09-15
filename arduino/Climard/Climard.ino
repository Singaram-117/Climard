#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define LDRPIN 33
#define blueLed 12
#define yellowLed 13
#define LED_PIN 2

DHT dht(DHTPIN, DHTTYPE);

float tempThreshold = 30.0;
int ldrThreshold = 500;

const char* baseURL = "http://192.168.0.103:8000/api/";
const char* receiveDataEndpoint = "receive_data/";
const char* getThresholdsEndpoint = "get_thresholds/";

const char* ssidList[] = {"UpStream", "Redmi K20", "SSN"};
const char* passwordList[] = {"upstream7", "hellomeow", "Ssn1!Som2@Sase3#"};
const int numWiFiNetworks = sizeof(ssidList) / sizeof(ssidList[0]);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(blueLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(blueLed, LOW);
  digitalWrite(yellowLed, LOW);
  handleLED(0);

  WiFi.mode(WIFI_STA);
  checkWiFiConnection();  // Ensure WiFi is connected
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int ldrValue = analogRead(LDRPIN);

  if (!isnan(humidity) && !isnan(temperature)) {
    if (sendSensorData(temperature, humidity, ldrValue)) {
      getThresholds();  // Fetch thresholds from the server
      controlLEDs(temperature, ldrValue);
    } else {
      handleLED(2);  // Blink LED if unable to communicate
    }
  }

  delay(100); // Delay before next loop iteration
}

bool sendSensorData(float temperature, float humidity, int ldrValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(baseURL) + receiveDataEndpoint;
    http.begin(url);

    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["ldr"] = ldrValue;
    
    String jsonData;
    serializeJson(doc, jsonData);

    Serial.println("Sending POST request:");
    Serial.println(jsonData);

    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonData);
    Serial.printf("Response code: %d\n", httpResponseCode);
    http.end();

    if (httpResponseCode == 200) {
      handleLED(1);  // Turn LED on if data sent successfully
      return true;
    } else {
      Serial.println("Failed to send data");
    }
  }
  return false;
}

void getThresholds() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(baseURL) + getThresholdsEndpoint;
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();
      StaticJsonDocument<200> doc;
      deserializeJson(doc, payload);

      tempThreshold = doc["temp_threshold"];
      ldrThreshold = doc["ldr_threshold"];
      Serial.printf("Updated thresholds: Temp: %.2f, LDR: %d\n", tempThreshold, ldrThreshold);
    } else {
      Serial.println("Failed to fetch thresholds");
    }

    http.end();
  }
}

void controlLEDs(float temperature, int ldrValue) {
  if (temperature > tempThreshold) {
    digitalWrite(blueLed, HIGH);
  } else {
    digitalWrite(blueLed, LOW);
  }

  if (ldrValue > ldrThreshold) {
    digitalWrite(yellowLed, HIGH);
  } else {
    digitalWrite(yellowLed, LOW);
  }
}

void checkWiFiConnection() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to WiFi...");

    for (int i = 0; i < numWiFiNetworks; i++) {
      WiFi.begin(ssidList[i], passwordList[i]);

      int attempt = 0;
      while (WiFi.status() != WL_CONNECTED && attempt < 20) {
        delay(100);
        Serial.print(".");
        attempt++;
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi!");
        handleLED(1);  // Turn LED on when connected
        return;
      }
    }

    Serial.println("\nFailed to connect, retrying...");
    delay(100);
  }
}

void handleLED(int state) {
  if (state == 0) {
    digitalWrite(LED_PIN, LOW);
  } else if (state == 1) {
    digitalWrite(LED_PIN, HIGH);
  } else if (state == 2) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

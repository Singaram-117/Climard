#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>

const char* ssid = "Bravo";
const char* password = "bravo220";

const char* serverName = "http://127.0.0.1:8000//register_esp32/";

WebServer server(80);

const int ledPin = 2;  // Built-in LED pin

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Register the ESP32 IP address with the Django server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "ip=" + WiFi.localIP().toString();
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Define routes
  server.on("/led_on", []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED is ON");
  });

  server.on("/led_off", []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/plain", "LED is OFF");
  });

  // Start server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();
}

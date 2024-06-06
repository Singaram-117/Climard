#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPAsyncWebSrv.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

const char* serverName = "http://your_django_server_address/register_esp32/";

AsyncWebServer server(80);

const int ledPin = 2;  // Built-in LED pin

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

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

  server.on("/led_on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);
    request->send(200, "text/plain", "LED is ON");
  });

  server.on("/led_off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);
    request->send(200, "text/plain", "LED is OFF");
  });

  server.begin();
}

void loop() {
  // Nothing to do here
}

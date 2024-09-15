#include <WiFi.h>
#include <HTTPClient.h>

const int LED_PIN = 2;  // Built-in LED pin
const char* ssidList[] = {"UpStream", "Redmi K20"};  // List of SSIDs
const char* passwordList[] = {"upstream7", "hellomeow"};  // Corresponding passwords
const int numWiFiNetworks = sizeof(ssidList) / sizeof(ssidList[0]);  // Number of WiFi networks

const char* serverURL = "https://singaram.pythonanywhere.com";  // HTTP request URL

// Function to handle LED states
void handleLED(int state) {
  if (state == 0) {
    digitalWrite(LED_PIN, LOW);  // Turn LED off
  } else if (state == 1) {
    digitalWrite(LED_PIN, HIGH);  // Turn LED on
  } else if (state == 2) {
    // Blink LED continuously
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  handleLED(0);  // Turn LED off initially
  Serial.begin(115200);      // Start serial communication
  WiFi.mode(WIFI_STA);       // Set ESP32 to station mode
}

void tryWiFiConnection() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to WiFi...");
    
    // Iterate through all SSIDs
    for (int i = 0; i < numWiFiNetworks; i++) {
      WiFi.begin(ssidList[i], passwordList[i]);

      // Wait for connection with timeout
      int attempt = 0;
      while (WiFi.status() != WL_CONNECTED && attempt < 20) {
        handleLED(0);  // LED off while trying
        delay(200);
        Serial.print(".");
        attempt++;
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi!");
        return;  // Exit if connected
      }
    }
    
    // Retry if not connected
    Serial.println("\nFailed to connect, retrying...");
    delay(200);  // Retry every 0.2 seconds
  }
}

void tryHttpRequest() {
  HTTPClient http;
  bool requestSuccessful = false;

  while (WiFi.status() == WL_CONNECTED) {  // Check if connected to WiFi
    handleLED(2);  // Blink LED while attempting request
    Serial.println("Sending HTTP request...");

    http.begin(serverURL);  // Specify the URL
    int httpCode = http.GET();  // Send the request

    if (httpCode == 200) {  // If HTTP response is OK
      Serial.println("HTTP request successful!");
      handleLED(1);  // Turn LED on
      requestSuccessful = true;
    } else {
      Serial.printf("Failed HTTP request, Code: %d\n", httpCode);
    }

    http.end();  // Close connection

    if (requestSuccessful) {
      return;  // Stop further processing
    }
    // Retry after some delay if request failed
    delay(200);
  }
}

void loop() {
  // Check if connected to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    handleLED(0);  // Turn LED off when disconnected
    tryWiFiConnection();  // Try to reconnect to WiFi
    if (WiFi.status() == WL_CONNECTED) {
      tryHttpRequest();  // Try HTTP request if connected
    }
  }
}

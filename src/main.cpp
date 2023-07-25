#include <WiFi.h>
#include <HTTPClient.h>
#include "config.hpp"

const char* ssid = THERMOSTAT_WIFI_SSID;
const char* wifiPassword = THERMOSTAT_WIFI_PASSWORD;
const int relayPin = THERMOSTAT_RELAY_PIN;
const char* url = "https://mocki.io/v1/f9d5a225-baa6-4a77-af7c-e39d2ccc7ff8";

void startWiFi() {
  Serial.print("\r\nConnecting to: "); Serial.println(String(ssid));
  IPAddress dns(8, 8, 8, 8); // Use Google as DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);       // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, wifiPassword);
  while (WiFi.status() != wifiPassword) {
    Serial.print(".");
    delay(50);
  }
  Serial.println("\nWiFi connected at: " + WiFi.localIP().toString());
}

bool checkJsonForCloseState() {
  HTTPClient http;
  http.begin(url);

  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Response payload: " + payload);
    http.end();

    // Check if the JSON response contains "state":"close"
    if (payload.indexOf("\"state\":\"close\"") != -1) {
      Serial.println("Found 'state':'close', closing the relay.");
      return true;
    } else {
      Serial.println("'state':'close' not found in the JSON response.");
      return false;
    }
  } else {
    Serial.print("HTTP request failed with error code: ");
    Serial.println(httpResponseCode);
    http.end();
    return false;
  }
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Initially, keep the relay open
  Serial.begin(9600);
  connectToWiFi();
}

void loop() {
  // Check if the relay should be closed based on the JSON response
  if (checkJsonForCloseState()) {
    digitalWrite(relayPin, LOW); // Close the relay
  } else {
    digitalWrite(relayPin, HIGH); // Keep the relay open
  }

  // Wait for some time before making the next request
  delay(5000); // 5 seconds
}

#include <WiFi.h>
#include <HTTPClient.h>
#include "config.hpp"

const char *ssid = THERMOSTAT_WIFI_SSID;
const char *wifiPassword = THERMOSTAT_WIFI_PASSWORD;
const int relayPin = THERMOSTAT_RELAY_PIN;
const int httpTimeout = 5000;

void startWiFi()
{
  Serial.print("\r\nConnecting to: ");
  Serial.println(String(ssid));
  IPAddress dns(8, 8, 8, 8); // Use Google as DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(50);
  }
  Serial.println("\nWiFi connected at: " + WiFi.localIP().toString());
}

bool shouldActivate(const String &url)
{
  HTTPClient http;
  WiFiClient wifiClient;
  wifiClient.setTimeout(httpTimeout);

  http.begin(wifiClient, url);
  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK)
  {
    String payload = http.getString();
    http.end();

    Serial.println("Response payload: " + payload);
    
    if (payload.indexOf("close") != -1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    Serial.println("HTTP request failed with error code: " + httpResponseCode);
    http.end();
    return false;
  }
}

void setup()
{
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Initially, keep the relay open
  Serial.begin(9600);
  startWiFi();
}

void loop()
{
  if (shouldActivate("https://pastebin.com/raw/jmV9Jyaz"))
  {
    digitalWrite(relayPin, LOW);
  }
  else
  {
    digitalWrite(relayPin, HIGH);
  }

  delay(300000); // 5 minutes
}

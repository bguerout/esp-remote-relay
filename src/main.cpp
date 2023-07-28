#include <WiFi.h>
#include <HTTPClient.h>
#include "config.hpp"
#include <UniversalTelegramBot.h>

const char *WIFI_SSID = JOUSSEHOME_WIFI_SSID;
const char *WIFI_PASSWORD = JOUSSEHOME_WIFI_PASSWORD;
const int RELAY_PIN = JOUSSEHOME_RELAY_PIN;
const char *BOT_TOKEN = JOUSSEHOME_TELEGRAM_BOT_TOKEN;
const String BOT_CHAT_ID = "6073873160";
const unsigned long MAX_WATERING_DURATION_MS = 300000;

WiFiClientSecure wifiClient;
UniversalTelegramBot bot(BOT_TOKEN, wifiClient);
unsigned long wateringStartedAt = 0;

void startWiFi()
{
  Serial.print("\r\nConnecting to: ");
  Serial.println(String(WIFI_SSID));
  IPAddress dns(8, 8, 8, 8); // Use Google as DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(50);
  }
  Serial.println("\nWiFi connected at: " + WiFi.localIP().toString());

  wifiClient.setCACert(TELEGRAM_CERTIFICATE_ROOT);
}

void startWatering()
{
  digitalWrite(RELAY_PIN, LOW);
  unsigned long now = millis();
  wateringStartedAt = now;

  bot.sendMessage(BOT_CHAT_ID, "Début de l'arrosage", "");
}

void stopWatering()
{
  digitalWrite(RELAY_PIN, HIGH);
  wateringStartedAt = 0;

  bot.sendMessage(BOT_CHAT_ID, "Fin de l'arrosage", "");
}

void readTelegramMessages()
{
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    String from_id = bot.messages[i].from_id;

    if (chat_id != BOT_CHAT_ID)
    {
      Serial.println("Utilisateur inconnu");
      return;
    }

    if (text == "/jardin start")
    {
      startWatering();
    }

    if (text == "/jardin stop")
    {
      stopWatering();
    }

    if (text == "/jardin status")
    {
      if (wateringStartedAt == 0)
      {
        bot.sendMessage(chat_id, "Pas d'arrosage actuellement", "");
      }
      else
      {
        unsigned long now = millis();
        unsigned long duration = (now - wateringStartedAt) / 1000;
        bot.sendMessage(chat_id, "Arrosage en cours depuis " + String(duration) + "s", "");
      }
    }
  }
}

void setup()
{
  pinMode(RELAY_PIN, OUTPUT);
  stopWatering();
  Serial.begin(9600);
  startWiFi();
}

void loop()
{
  unsigned long now = millis();

  if (wateringStartedAt != 0 && now - wateringStartedAt > MAX_WATERING_DURATION_MS)
  {
    stopWatering();
  }
  else
  {
    readTelegramMessages();
  }

  delay(5000); // 1 minute
}

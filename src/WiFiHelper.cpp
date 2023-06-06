#include "WiFiHelper.h"

void WiFiHelper::connectToWiFi(WiFiClient &espClient, PubSubClient &client)
{
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  delay(1000);
  // Connect to WiFi
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

bool WiFiHelper::getWiFistatus(WiFiClient &espClient, PubSubClient &client)
{
  // Handle WiFi and MQTT in the loop
  if (WiFi.status() != WL_CONNECTED || !client.connected())
  {
    return false;
  }
  return true;
}

#include "WiFiHelper.h"

void connectToWiFi(WiFiClient& espClient, PubSubClient& client) {
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  delay(1000);
  // Connect to WiFi
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  

}

// void callback(char* topic, byte* payload, unsigned int length) {
//   // Handle MQTT message received
// }

// void setupWiFi(WiFiClient& espClient, PubSubClient& client, const char* ssid, const char* password) {
//   // Initialize Serial and WiFi
//   Serial.begin(115200);
//   WiFi.mode(WIFI_AP_STA);
//   WiFi.disconnect();
//   delay(1000);

//   // Connect to WiFi and MQTT
//   connectToWiFi(espClient, client, ssid, password);
// }

bool loopWiFi(WiFiClient& espClient, PubSubClient& client) {
  // Handle WiFi and MQTT in the loop
  if (WiFi.status() != WL_CONNECTED || !client.connected()) {
    return false;
  }
  client.loop();
  return true;
}

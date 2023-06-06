#ifndef WiFiHelper_H
#define WiFiHelper_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define _SSID "AILab"        // Your WiFi SSID
#define _PASSWORD "ailab120"    // Your WiFi Password

void connectToWiFi(WiFiClient& espClient, PubSubClient& client);
void callback(char* topic, byte* payload, unsigned int length);
void setupWiFi(WiFiClient& espClient, PubSubClient& client, const char* ssid, const char* password);
bool loopWiFi(WiFiClient& espClient, PubSubClient& client);

#endif

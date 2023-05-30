#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

class MQTT {
private:
  const char* broker;
  const int port;
  WiFiClient espClient;
  PubSubClient client;

public:
  MQTT(const char* broker, int port);
  void connect();
  void disconnect();
  bool isConnected();
  void subscribe(const char* topic);
  void publish(const char* topic, const char* message);
  void loop();
};

#endif

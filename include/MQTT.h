#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>



class MQTT {
private:
  static const char* broker; //IP address of broker
  static const int port;
  WiFiClient espClient;
  PubSubClient client;

public:
  MQTT();
  void connect(const char* mqtt_name);
  void disconnect();
  bool isConnected();
  void subscribe(const char* topic);
  void publish(const char* topic, const char* message);
  void loop();
};


#endif

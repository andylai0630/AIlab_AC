#include "MQTT.h"



MQTT::MQTT(const char* broker, int port)
  : broker(broker), port(port), client(espClient) {}

void MQTT::connect() {
  client.setServer(broker, port);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("AC_Smart_B_121_2")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(500);
    }
  }
}

void MQTT::disconnect() {
  client.disconnect();
}

bool MQTT::isConnected() {
  return client.connected();
}

void MQTT::subscribe(const char* topic) {
  client.subscribe(topic);
  Serial.println("Subscribe MQTT topic" + String(topic));
}

void MQTT::publish(const char* topic, const char* message) {
  client.publish(topic, message);
}

void MQTT::loop() {
  client.loop();
}

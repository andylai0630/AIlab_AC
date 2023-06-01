#include "MQTT.h"

const char* MQTT::broker = "140.123.106.232";
const int MQTT::port = 1883;

MQTT::MQTT()
  : client(espClient) {}

void MQTT::connect(const char* mqtt_name) {
  client.setServer(broker, port);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(mqtt_name)) {
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

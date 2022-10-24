#include <ESP8266WiFi.h>
#include "mqtt.h"
#include <WifiClient.h>
#include "config.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

WiFiClient net;
PubSubClient client(net);
StaticJsonDocument<200> doc;
relayHandlerType relayHandler = NULL;

char buffer[100];

void connectMqtt() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("zasuvka")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      sprintf(buffer, "Zasuvka connected to MQTT with IP  %s, dedicated log %s", WiFi.localIP().toString().c_str(), MQTT_LOG_TOPIC);
      client.publish(MQTT_GLOBAL_LOG_TOPIC,buffer);
      mqttLog(buffer);
      // ... and resubscribe
      client.subscribe(MQTT_SUB_TOPIC);
    } else {
      Serial.print("Mqtt connect failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqttHeartBeat() {
    mqttLog("Zasuvka alive");
}

void logState(int value) {
  if (client.connected()) {
    sprintf(buffer, "Zasuvka state  %d", value);
    client.publish(MQTT_LOG_TOPIC,buffer);
  }
}

void mqttLog(char *message) {
  if (client.connected()) {
    client.publish(MQTT_LOG_TOPIC,message);
  }
}

void setRelayHandler(relayHandlerType handler) {
    relayHandler = handler;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  u8 val = 0;
  u8 valSet = 0;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    if (strncmp("ON", (char *) payload, 2) == 0) {
      val = 1;
      valSet = 1;
    }
    if (strncmp("OFF", (char *) payload, 3) == 0) {
      val = 0;
      valSet = 1;
    }
    if (!valSet) {
      Serial.println("Non-json content not recognized (expected ON/OFF)");
      return;
    }
  } else {
    val = doc["value"];
    valSet = 1;
  }
  if (valSet) {
      Serial.print("Setting relay to ");
      Serial.println(val);
      if (relayHandler != NULL) {
          relayHandler(val);
      } else {
          Serial.println("WARNING: relay handler not set");
      }
      if (client.connected()) {
        client.publish(MQTT_PUB_TOPIC, val ? "ON" : "OFF");
      }
  }
}

void setupMqtt() {
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
}

void mqttLoop() {
  if (!client.connected()) {
    connectMqtt();
  }
  client.loop();
}
#ifndef MQTT_H
#define MQTT_H

typedef void (*relayHandlerType)(u8);

void mqttHeartBeat();
void setupMqtt();
void connectMqtt();
void mqttLoop();
void mqttLog(char *message);
void setRelayHandler(relayHandlerType handler);
void logState(int value);

#endif
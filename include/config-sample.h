// rename this file to config.h and update values below

#ifndef CONFIG_H
#define CONFIG_H

// control topic to listen to - with ON/OFF commands
#define MQTT_SUB_TOPIC "/devices/zasuvka1/set"
// status topic
#define MQTT_PUB_TOPIC "/devices/zasuvka1"
// logging topic
#define MQTT_LOG_TOPIC "/devices/zasuvka1/log"
// logging topic to send IP once
#define MQTT_GLOBAL_LOG_TOPIC "log"
#define WIFI_SSID "my_network"
#define WIFI_PASSWORD "my_password"
#define MQTT_SERVER "192.168.1.1"
// interval in seconds to send heartbeat message
#define HEART_BEAT_S 60*60
// interval in seconds to blink built-in LED
#define BLICK_S 30

// Wemos D1
#define PIN_RELAY 5 


#endif

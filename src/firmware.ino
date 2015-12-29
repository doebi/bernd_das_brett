/*
 * Firmware for an ESP powering a phyiscal watergun becoming a browsergame..
 * (c) by Christoph (doebi) Döberl
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>

String nodeName = "devlol/babyscooter/";

// pins
const int left = 14;
const int right = 15;

int isAlive = 0;
int mil = 0;

char state = 0;

ESP8266WiFiMulti wifiMulti;

IPAddress MQTTserver(158, 255, 212, 248);
WiFiClient wclient;
PubSubClient client(wclient, MQTTserver);

void mqtt_callback(const MQTT::Publish& pub) {
    String topic = pub.topic();
    String msg = pub.payload_string();
    if(topic == "devlol/babyscooter/left") {
        digitalWrite(left, msg == "0" ? LOW : HIGH);
    }
    if(topic == "devlol/babyscooter/right") {
        digitalWrite(right, msg == "0" ? LOW : HIGH);
    }
}

void setup() {
    Serial.begin(115200);
    delay(10);

    isAlive = millis();

    wifiMulti.addAP("/dev/lol", "4dprinter");
    wifiMulti.addAP("ESP-Spielwiese", "ovomaltine");
    wifiMulti.addAP("DildoAP", "dildo123");
    wifiMulti.addAP("32C3-open-legacy", "");

    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("Wifi connected");
    }

    pinMode(left, OUTPUT);
    pinMode(right, OUTPUT);
}

void loop() {
    // wifi
    if(WiFi.status() != WL_CONNECTED) {
        if(wifiMulti.run() == WL_CONNECTED) {
            Serial.println("Wifi connected");
        }
    }

    // mqtt
    if (client.connected()) {
        client.loop();
    } else {
        if (client.connect("babyscooter", nodeName + "online", 0, true, "false")) {
            client.publish(nodeName + "online", "true", true);
            Serial.println("MQTT connected");
            client.set_callback(mqtt_callback);
            client.subscribe("#");
        }
    }
}

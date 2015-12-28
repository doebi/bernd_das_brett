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
const int motor = 5;

int motorState = LOW;
int motorStart = 0;

int isAlive = millis();
int mil = 0;

ESP8266WiFiMulti wifiMulti;

IPAddress MQTTserver(158, 255, 212, 248);
WiFiClient wclient;
PubSubClient client(wclient, MQTTserver);

void mqtt_callback(const MQTT::Publish& pub) {
  String topic = pub.topic();
  String msg = pub.payload_string();
  if(topic == "doebi/watergun/button") {
    if (msg == "DOWN") {
      motorStart = millis();
      motorState = HIGH;
      digitalWrite(motor, HIGH);
    }
    if (msg == "UP ") {
      motorState = LOW;
      digitalWrite(motor, LOW);
    }
  }
}

void setup() {
    Serial.begin(115200);
    delay(10);
	
    wifiMulti.addAP("/dev/lol", "4dprinter");
    wifiMulti.addAP("ESP-Spielwiese", "ovomaltine");
    wifiMulti.addAP("DildoAP", "dildo123");
    wifiMulti.addAP("32C3-open-legacy", "");
    
    if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("Wifi connected");
    }
    
    pinMode(motor, OUTPUT);
}

void loop() {
  // heartbeat
  mil = millis();
  if(mil - isAlive >= 30000) {
    client.publish(nodeName + "heartbeat", (String)mil);
    isAlive = mil;  
  }
  
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

#include <WiFi.h>
#include <MQTT.h>
#include "LIS3DHTR.h"
#include <Wire.h>

// ---------------- WIFI ----------------
const char ssid[] = "IoT_course";
const char password[] = "IOTisMAGIC777";

// ---------------- MQTT ----------------
const char mqtt_broker[] = "broker.hivemq.com"; 
const char topic[] = "wio/horizon";

WiFiClient net;
MQTTClient client(512);

// ---------------- SENSOR ----------------
LIS3DHTR<TwoWire> lis;

// ---------------- TIMING ----------------
unsigned long lastSend = 0;

// ---------------- WIFI ----------------
void connectWiFi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
}

// ---------------- MQTT ----------------
void connectMQTT() {
  Serial.print("Connecting MQTT");

  while (!client.connect("WIO_TERMINAL_HORIZON")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nMQTT connected");
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  lis.begin(Wire1);
  lis.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

  connectWiFi();
  client.begin(mqtt_broker, net);
  connectMQTT();
}

// ---------------- LOOP ----------------
void loop() {

  client.loop();

  if (!client.connected()) {
    connectMQTT();
  }

  // ---------------- ACCEL ----------------
  float ax = lis.getAccelerationX();
  float ay = lis.getAccelerationY();
  float az = lis.getAccelerationZ();

  // ---------------- ATTITUDE ----------------
  float roll  = atan2(ay, az) * 57.2958;
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 57.2958;

  // ---------------- G FORCE ----------------
  float gForce = sqrt(ax * ax + ay * ay + az * az);

  // ---------------- SEND ----------------
  if (millis() - lastSend > 50) {
    lastSend = millis();

    char msg[160];

    sprintf(msg,
      "{\"roll\":%.2f,\"pitch\":%.2f,\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f}",
      roll, pitch, ax, ay, az
    );

    client.publish(topic, msg);

    Serial.println(msg);
  }
}
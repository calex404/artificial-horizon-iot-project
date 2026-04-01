#include <WiFi.h>
#include <MQTT.h>
#include "LIS3DHTR.h"
#include <Wire.h>

const char ssid[] = "IoT_course";  // WiFi network
const char password[] = "IOTisMAGIC777";

const char mqtt_broker[] = "broker.hivemq.com";  // MQTT server
const char topic[] = "wio/horizon";

WiFiClient net;
MQTTClient client(512);

LIS3DHTR<TwoWire> lis;  // built-in accelerometer

unsigned long lastSend = 0;

void connectWiFi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi successfully connected");
}

void connectMQTT() {
  Serial.print("Connecting MQTT");

  while (!client.connect("WIO_TERMINAL_HORIZON")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nMQTT successfully connected");
}

void setup() {
  Serial.begin(115200); // initializes USB serial communication
  delay(1000);

  lis.begin(Wire1);  // initializes the accelerometer
  lis.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

  connectWiFi();
  client.begin(mqtt_broker, net); // link MQTT client to the broker address
  connectMQTT();
}

void loop() {

  client.loop(); // keeps MQTT connection

  if (!client.connected()) {
    connectMQTT();
  }

  float ax = lis.getAccelerationX();  // read acceleration data
  float ay = lis.getAccelerationY();
  float az = lis.getAccelerationZ();

  float roll  = atan2(ay, az) * 57.2958;  // Calculate roll and pitch
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 57.2958;

  float gForce = sqrt(ax * ax + ay * ay + az * az);  // calculating G

  if (millis() - lastSend > 50) {
    lastSend = millis();

    char msg[160];

    sprintf(msg,  // data conversion
      "{\"roll\":%.2f,\"pitch\":%.2f,\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f}",
      roll, pitch, ax, ay, az
    );

    client.publish(topic, msg);  // sending data to the broker

    Serial.println(msg);
  }
}
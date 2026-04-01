#include <WiFi.h>
#include <MQTT.h>
#include "LIS3DHTR.h"
#include <Wire.h>

const char wifiName[] = "IoT_course";  // WiFi
const char password[] = "IOTisMAGIC777";

const char brokerAddress[] = "broker.hivemq.com";  // MQTT
const char topic[] = "wio/horizon";

WiFiClient net;
MQTTClient client(512);

LIS3DHTR<TwoWire> accelerometer;  // built-in accelerometer in Wio Terminal

unsigned long lastSend = 0;


void connectWiFi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(wifiName, password);

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
  Serial.begin(115200);  // initializes USB serial communication
  delay(1000);

  accelerometer.begin(Wire1);  // initializes the accelerometer
  accelerometer.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

  connectWiFi();
  client.begin(brokerAddress, net);
  connectMQTT();
}

void loop() {
  client.loop();  // keeps MQTT connection alive

  if (!client.connected()) {
    connectMQTT();
  }

  float ax = accelerometer.getAccelerationX();    // read acceleration data
  float ay = accelerometer.getAccelerationY();
  float az = accelerometer.getAccelerationZ();    
  float roll  = atan2(ay, az) * 57.2958;    // calculate roll and pitch — 57.2958 = 180/pi (rad to deg)
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 57.2958;
  float gForce = sqrt(ax * ax + ay * ay + az * az);  // calculating G

  if (millis() - lastSend > 50) {    
    lastSend = millis();

    char message[160];
    sprintf(message,
      "{\"roll\":%.2f,\"pitch\":%.2f,\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f}",
      roll, pitch, ax, ay, az
    );

    client.publish(topic, message);  // sending data to the broker
    Serial.println(message);
  }
}
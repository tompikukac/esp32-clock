#include <Arduino.h>

#include "wifi/wifi_controller.h"
#include "deepsleep.cpp"

#define LED 2

WifiController* wifi;
DeepSleep deepSleep;

void setup() {
  Serial.begin(115200);
  Serial.println("TEMPERATURE");
  pinMode(LED,OUTPUT);

  Serial.println("WiFi connecting...");
  // wifi = new WifiController();

  // if (wifi->connect()) {
  //   Serial.println("WiFi connected!");
  //   Serial.println(WiFi.localIP());
  //   Serial.println(wifi->getDeviceId());

  // } else {
  //     Serial.println("WiFi connection failed");
  //     deepSleep.sleep(0,5);
  // }
}

void loop() {
  Serial.println("LOOP");
  delay(1500);
  digitalWrite(LED,HIGH);
  delay(1500);
  digitalWrite(LED,LOW);
}

#include <Arduino.h>

#include "wifi/wifi_controller.h"
#include "deepsleep.cpp"
#include "config.cpp"
#include "internet/internet_services.cpp"

#define LED 2
#define CONFIG_ROOT "https://raw.githubusercontent.com/tompikukac/esp32-projects/main/config/devices/"

WifiController* wifi;
InternetServices net;
Config config;
DeepSleep deepSleep;

void setup() {
  Serial.begin(115200);
  Serial.println("TEMPERATURE");
  pinMode(LED,OUTPUT);

  Serial.println("WiFi connecting...");
  wifi = new WifiController();

  if (wifi->connect()) {
    Serial.println("WiFi connected!");
    Serial.println(WiFi.localIP());
    Serial.println(wifi->getDeviceId());

    // Load configuration
    String defaultJson = net.getConfig(String(CONFIG_ROOT) + "default.json");
    config.parse(defaultJson);
    String configJson = net.getConfig(CONFIG_ROOT + wifi->getDeviceId() + ".json");
    config.parse(configJson);
    Serial.println("Device name: " + config.name);

  } else {
      Serial.println("WiFi connection failed");
      deepSleep.sleep(0,5);
  }
}

void loop() {
  Serial.println("LOOP");
  delay(1500);
  digitalWrite(LED,HIGH);
  delay(1500);
  digitalWrite(LED,LOW);
}

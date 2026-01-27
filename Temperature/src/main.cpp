#include <Arduino.h>
#include "esp32_c3_zero_led.cpp"

#include "wifi/wifi_controller.h"
#include "deepsleep.cpp"
#include "config.cpp"
#include "internet/internet_services.cpp"
#include "config_storage.cpp"

#define CONFIG_ROOT "https://raw.githubusercontent.com/tompikukac/esp32-projects/main/config/devices/"

Esp32C3ZeroLed statusLed(10);

WifiController* wifi;
InternetServices net;
Config config;
DeepSleep deepSleep;
ConfigStorage storage;

void setup() {
  Serial.begin(115200);
  Serial.println("TEMPERATURE");
  delay(1000);
  statusLed.begin();
  statusLed.setBrightness(4);


  Serial.println("WiFi connecting...");
  wifi = new WifiController();

  if (wifi->connect()) {
    Serial.println("WiFi connected!");
    Serial.println(WiFi.localIP());
    Serial.println(wifi->getDeviceId());

    storage.begin();

    if (!storage.loadConfig(config)) {
      // Load configuration
      Serial.println("Reading default configuration...");
      String defaultJson = net.getConfig(String(CONFIG_ROOT) + "default.json");
      config.parse(defaultJson);
      Serial.println("Reading specific configuration...");
      String configJson = net.getConfig(CONFIG_ROOT + wifi->getDeviceId() + ".json");
      Serial.println("Config JSON: " + configJson);
      config.parse(configJson);
    } else {
      Serial.println("Configuration loaded from storage.");
    }

    if(config.name != "default-device") {
        Serial.println("Saving configuration...");
        storage.saveConfig(config);
    }
    storage.end();

    Serial.println(config.toString());

  } else {
      Serial.println("WiFi connection failed");
      deepSleep.sleep(0,5);
  }
}

void loop() {
  Serial.println("LOOP...");
  statusLed.setColor(Colors::Olive);
  delay(1500); 
  statusLed.setColor(Colors::Cyan);
  delay(1500);
  Serial.println("...LOOP");

  // deepSleep.sleepInSec(config.deepSleepTimeInSec);
}


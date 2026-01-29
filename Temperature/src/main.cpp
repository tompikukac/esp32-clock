#include <Arduino.h>
#include "esp32_c3_zero_led.cpp"

#include "wifi/wifi_controller.h"
#include "deepsleep.cpp"
#include "config/config_data.h"
#include "config/config_controller.h"
#include "bme280_sensor.h"
#include "secret.h"
#include "influxdb_controller.cpp"

#define CONFIG_ROOT "https://raw.githubusercontent.com/tompikukac/esp32-projects/main/config/devices/"

InfluxController influx("http://192.168.1.111:8086", "szlab", "esp32", influxToken);

Esp32C3ZeroLed statusLed(10);

WifiController* wifi;
ConfigData config;
DeepSleep deepSleep;
BME280Sensor sensor(4, 5);

void goToDeepSleep(const LedColor* color, uint32_t sleepSeconds) {
  if (color == nullptr) {
    statusLed.off();
  } else {
    statusLed.setColor(*color);
  }
  Serial.flush();
  delay(100);
  wifi->disconnect();
  deepSleep.sleepInSec(sleepSeconds);
}
#define BOOT_BUTTON_GPIO 0

void setup() {
  statusLed.begin();
  statusLed.setBrightness(4);
  statusLed.setColor(Colors::Blue);  

  Serial.begin(115200);
  delay(2000);

  Serial.println("TEMPERATURE");
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  bool forceLoad = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED);
  Serial.printf("Wakeup cause: %d, forceLoad: %d\n", cause, forceLoad);
  
  if (forceLoad) {
    Serial.println("safety delay for first time setup");
    statusLed.setColor(Colors::Magenta);  
    delay(15000);
  } else {
    delay(2000);
  }

  Serial.println("WiFi connecting...");
  wifi = new WifiController();

  if (wifi->connect()) {
    ConfigController configCtrl(wifi->getDeviceId(), String(CONFIG_ROOT));
    
    config = configCtrl.load(forceLoad);

    if (!sensor.begin()) {
      Serial.println("BME280 init failed!");
      goToDeepSleep(&Colors::Yellow, 10);
    }
    Serial.println("BME280 ready");
    
    Serial.println("Config: " + config.toString());

  } else {
      Serial.println("WiFi connection failed");
      goToDeepSleep(&Colors::Red, 10);
  }
  statusLed.setColor(Colors::Lime);  
}

void loop() {
  Serial.println("LOOP...");
  statusLed.setColor(Colors::Olive);
  delay(500); 
  statusLed.setColor(Colors::Green);
  delay(500);
 

  BME280Data data = sensor.read();
  Serial.printf("T: %.2f C, H: %.2f %%, P: %.2f hPa\n", data.temperature, data.humidity, data.pressure);
 
  statusLed.setColor(Colors::Blue);
  delay(500); 
  influx.send(data, config.name);

  statusLed.setColor(Colors::Green);
  delay(500); 
  statusLed.off();
  delay(100); 

  goToDeepSleep(nullptr, config.deepSleepTimeInSec);
}

#include <Arduino.h>
#include "esp32_c3_zero_led.cpp"

#include "wifi/wifi_controller.h"
#include "deepsleep.cpp"
#include "config/config_data.h"
#include "config/config_controller.h"
#include "bme280_sensor.h"
#include "secret.h"
#include "influxdb_controller.cpp"
#include "logger.h"
#include <esp_task_wdt.h>

#define CONFIG_ROOT "https://raw.githubusercontent.com/tompikukac/esp32-projects/main/Temperature/config/devices/"

InfluxController influx("http://192.168.1.111:8086", "szlab", "esp32", influxToken);

Esp32C3ZeroLed statusLed(10,4);

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
  wifi->disconnect();
  delay(500);

  deepSleep.sleepInSec(sleepSeconds);
}
#define BOOT_BUTTON_GPIO 0

void setup() {
  // 1. Start the Watchdog immediately
  // If we don't reach the deep_sleep_start within X, REBOOT
  esp_task_wdt_init(60, true); 
  esp_task_wdt_add(NULL);

  delay(200);
  statusLed.begin();
  statusLed.setColor(Colors::Magenta);
 
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  bool forceLoad = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED);

  if (forceLoad) {
    uint32_t delayMs = esp_random() % 5000;
    delay(12000 + delayMs);
  } else {
    delay(2000);
  }

   statusLed.setColor(Colors::Red); 
  logger.begin(115200, 5000);
  statusLed.setColor(Colors::Blue); 
  logger.println("TEMPERATURE");
  logger.printf("Wakeup cause: %d, forceLoad: %d\n", cause, forceLoad);
  logger.println("WiFi connecting...");

  ConfigController configCtrl(wifi->getDeviceId(), String(CONFIG_ROOT));
  ConfigData* cfg = configCtrl.load();

  // WIFI
  wifi = new WifiController();
  if (!wifi->connect(cfg->ip)) {
      influx.sendLog("WiFi connection failed", config.name);
      goToDeepSleep(&Colors::Red, 10);
  }

  if (cfg == nullptr || forceLoad) {
    influx.sendLog("Force loading config", config.name);
    config = configCtrl.forceLoad();
  } else {
    config = *cfg;
  }

  if (config.name == "default-device") {
    influx.sendLog("not able to load config for: " + wifi->getDeviceId(), "default-device");
    goToDeepSleep(&Colors::Yellow, 60);
  }
  logger.println("Config: " + config.toString());
  delay(50); 

  statusLed.setColor(Colors::Lime);
  
  // SENSOR
  if (!sensor.begin()) {
    influx.sendLog("BME280 init failed!", config.name);
    goToDeepSleep(&Colors::Yellow, 10);
  }
  logger.println("BME280 ready");

  statusLed.setColor(Colors::Olive);
 
  // Read sensor
  BME280Data* data = sensor.read();
  if (data == nullptr) {
    statusLed.setColor(Colors::Blue);
    influx.sendLog("BME280 read failed", config.name);
    goToDeepSleep(&Colors::Yellow, 10);
  }
  logger.printf("T: %.2f C, H: %.2f %%, P: %.2f hPa\n", data->temperature, data->humidity, data->pressure);

  statusLed.setColor(Colors::Green);

  // Send to InfluxDB
  if(! influx.send(*data, config.name)) {
    logger.println("InfluxDB send failed");
    goToDeepSleep(&Colors::Red, 10);
  }

  statusLed.setColor(Colors::Blue);
  delay(200); 
  statusLed.setColor(Colors::Green);
  delay(200); 
  statusLed.off();
  delay(100); 

  // delay(config.deepSleepTimeInSec * 1000);
  goToDeepSleep(nullptr, config.deepSleepTimeInSec);
}

void loop() {
    // Not used
}

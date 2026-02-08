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
#include "oled/oled_display.h"
#include "sensor/temperature_sensor.h"
#include "sensor/sensor_factory.h"
#include <WiFi.h>

#define CONFIG_ROOT "https://raw.githubusercontent.com/tompikukac/esp32-projects/main/Temperature/config/devices/"

InfluxController influx("http://192.168.1.111:8086", "szlab", "esp32", influxToken);

Esp32C3ZeroLed statusLed(10,4);

WifiController* wifi;
ConfigData config;
DeepSleep deepSleep;
// BME280Sensor sensor(4, 5);

OledDisplay oled;

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

  // Serial.begin(115200);
  //         IPAddress local_IP(192,168,1,77); // from parameter
  //       IPAddress gateway(192,168,1,1);  // set your gateway
  //       IPAddress subnet(255,255,255,0); // set your subnet mask

  //       WiFi.config(local_IP, gateway, subnet);
  // WiFi.begin("VaultTec", "polpolpol");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("Connected");


  logger.printf("Wakeup cause: %d, forceLoad: %d\n", cause, forceLoad);
  
  logger.println("Getting configs...");
  ConfigController configCtrl(wifi->getDeviceId(), String(CONFIG_ROOT));
  ConfigData* cfg = configCtrl.load();
  
  // WIFI
  logger.println("WiFi connecting...");
  wifi = new WifiController();
  if (!wifi->connect(cfg ? &cfg->ip : nullptr)) {
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

  if (cfg->oled)
  {
    oled.begin();
    oled.setFont(u8g2_font_timB18_tn);
  }

  TemperatureSensor* sensor = createSensor(cfg->sensorType, Wire);
  
  // SENSOR
  if (!sensor || !sensor->begin()) {
  // if (!sensor.begin()) {
    influx.sendLog("sensor init failed!", config.name);
    goToDeepSleep(&Colors::Yellow, 10);
  }
  logger.println("Sensor ready");

  statusLed.setColor(Colors::Olive);
 
  // Read sensor
  TemperatureData data = sensor->read();

  if (!data.isValid()) {
    statusLed.setColor(Colors::Blue);
    influx.sendLog("BME280 read failed", config.name);
    goToDeepSleep(&Colors::Yellow, 10);
  }
  logger.printf("T: %.2f C, H: %.2f %%, P: %.2f hPa\n", data.temperature, data.humidity, data.pressure);

  statusLed.setColor(Colors::Green);


  if (cfg->oled)
  {
    String tempStr = String(data.temperature, 2);
    oled.setText(tempStr, 0, 30);
    oled.show();
  }

  // Send to InfluxDB
  if(! influx.send(data, config.name)) {
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

#include <Wire.h>
#include <Arduino.h>

#include "oled/oled_display.h"
#include "sensor/temperature_sensor.h"
#include "sensor/sensor_factory.h"

OledDisplay oled;
// WifiControllerB* wifi;
// WifiControllerB wifi;
// #include <Adafruit_BME280.h>
// #include <Adafruit_BME280.h>

// #include "esp_sleep.h"

// #define I2C_SDA 4
// #define I2C_SCL 5
// #define BME280_I2C_ADDR 0x76

// U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 6, /* data=*/ 5);

#define LED_PIN 8

TemperatureSensor* sensor = createSensor(SensorType::SHT30, Wire);
// TemperatureSensor* sensor = createSensor(SensorType::SHT40);


void setup() {
 Serial.begin(115200);
  delay(1000);

  // Wire.begin(I2C_SDA, I2C_SCL);
  
  // bool status = bme.begin(BME280_I2C_ADDR);
  // if (!status) {
  //   Serial.println("Nem sikerült inicializálni a BME280 szenzort!");
  //   while (1);
  // }
  // Serial.println("BME280 inicializálva.");

  pinMode(LED_PIN, OUTPUT);

  oled.begin();
  oled.setFont(u8g2_font_timB18_tn);
  // oled.setFont(u8g2_font_ncenB08_tr);

  delay(50);
  Serial.println("setup...");
}

void loop() {

   if (sensor && sensor->begin()) {

    if(sensor->isStarted()) {
      Serial.println("Szenzor sikeres.");
    } else {
      Serial.println("Szenzor sikertelen.");
    }
    
    TemperatureData data = sensor->read();
    String tempStr = String(data.temperature, 2);
    String humidityStr = String(data.humidity, 0) + "%";
    Serial.println("TEMP:" + tempStr + " HUM:" + humidityStr);
    oled.setText(tempStr, 0, 30);
    oled.show();
    digitalWrite(LED_PIN, HIGH);
    delay(5000);
    oled.setText(humidityStr+"a", 0, 30);
    oled.show();
    digitalWrite(LED_PIN, LOW);
    // oled.setText(humidityStr, 0, 40);
   } else {
    Serial.println("Szenzor inicializálása sikertelen!");
    oled.setText("Sensor error", 0, 30);
    oled.show();
   }
  

  // if (!mySensor.beginI2C(Wire)) {
  //   Serial.println("BME280 nem indul");
  // }
  //   Serial.println("BME280 inicializálva.");
  //   float temperature = mySensor.readTempC();
  //   float humidity = mySensor.readFloatHumidity();
  //   float pressure = mySensor.readFloatPressure() / 100.0F; // hPa-ba

    // Serial.print("Hőmérséklet: ");
    // Serial.print(temperature);
    // Serial.print(" °C, Páratartalom: ");
    // Serial.print(humidity);
    // Serial.print(" %, Légnyomás: ");
    // Serial.print(pressure);
    // Serial.println(" hPa");




  // // --- Init BME280 ---
  // if (!bme.begin(0x76)) {
  //   Serial.println("BME280 not found!");
  //  // esp_deep_sleep_start();
  // }

  // --- Force-mode, lowest power ---
  // bme.setSampling(
  //   Adafruit_BME280::MODE_FORCED,
  //   Adafruit_BME280::SAMPLING_X1,   // temperature
  //   Adafruit_BME280::SAMPLING_X1,   // pressure
  //   Adafruit_BME280::SAMPLING_X1,   // humidity
  //   Adafruit_BME280::FILTER_OFF
  // );

  // --- Let ESP32 settle thermally ---
  delay(300);   // critical if waking from deep sleep

  // --- Throw away first reading ---
  // bme.takeForcedMeasurement();
  delay(100);

  // // --- Actual measurement ---
  // bme.takeForcedMeasurement();

  // float temperature = bme.readTemperature();
  // float humidity    = bme.readHumidity();
  // float pressure    = bme.readPressure() / 100.0F;

  // Serial.printf("T=%.2f °C  H=%.2f %%  P=%.2f hPa\n",
  //               temperature, humidity, pressure);

  Serial.println("Looping...");
  delay(5000);

  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
  Serial.println("Looping...");


}

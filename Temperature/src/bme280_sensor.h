#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

struct BME280Data {
  float temperature;
  float humidity;
  float pressure;
};

class BME280Sensor {
public:
  BME280Sensor(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_addr = 0x76);

  bool begin();
  BME280Data* read();

private:
  uint8_t _sda, _scl, _addr;
  Adafruit_BME280 _bme;
};

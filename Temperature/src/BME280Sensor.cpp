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
  BME280Sensor(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_addr = 0x76)
    : _sda(sda_pin), _scl(scl_pin), _addr(i2c_addr) {}

  bool begin() {
    Wire.begin(_sda, _scl);
    return _bme.begin(_addr);
  }

  BME280Data read() {
    BME280Data data;
    data.temperature = _bme.readTemperature();
    data.humidity    = _bme.readHumidity();
    data.pressure    = _bme.readPressure() / 100.0F; // hPa
    return data;
  }

private:
  uint8_t _sda, _scl, _addr;
  Adafruit_BME280 _bme;
};

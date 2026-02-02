#include "bme280_sensor.h"

BME280Sensor::BME280Sensor(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_addr)
  : _sda(sda_pin), _scl(scl_pin), _addr(i2c_addr) {}

bool BME280Sensor::begin() {
  Wire.begin(_sda, _scl);
  Wire.setClock(100000);
  delay(10);

  for (int i = 0; i < 5; i++) {
    if (_bme.begin(_addr)) {
      return true;
    }
    delay(20);
  }
  return false;
}

BME280Data averageData(BME280Data data[], int count) {
  BME280Data avg = {0};
  for (int i = 0; i < count; i++) {
    avg.temperature += data[i].temperature;
    avg.humidity += data[i].humidity;
    avg.pressure += data[i].pressure;
  }
  avg.temperature /= count;
  avg.humidity /= count;
  avg.pressure /= count;
  return avg;
}

BME280Data* BME280Sensor::read( int delayMs ) {
  static BME280Data samples[5];
  int validCount = 0;
  int failureCount = 0;

  while (validCount < 5 && failureCount < 5) {
    float temp = _bme.readTemperature();
    float hum = _bme.readHumidity();
    float pres = _bme.readPressure() / 100.0F;

    if (temp > -40 && temp < 85 &&
        hum >= 0 && hum <= 100 &&
        pres > 300 && pres < 1100) {
      samples[validCount].temperature = temp;
      samples[validCount].humidity = hum;
      samples[validCount].pressure = pres;
      validCount++;
    } else {
      failureCount++;
    }
    delay(delayMs);
  }

  if (validCount == 5) {
    static BME280Data avg = averageData(samples, 5);
    return &avg;
  }
  return nullptr;
}

// BME280Data* BME280Sensor::read() {
//   static BME280Data data;

//   for (int i = 0; i < 5; i++) {
//     data.temperature = _bme.readTemperature();
//     data.humidity    = _bme.readHumidity();
//     data.pressure    = _bme.readPressure() / 100.0F; // hPa

//     if (data.temperature > -40 && data.temperature < 85 &&
//         data.humidity >= 0 && data.humidity <= 100 &&
//         data.pressure > 300 && data.pressure < 1100) {
//       return &data;
//     }

//     delay(5);
//   }

//   return nullptr;
// }

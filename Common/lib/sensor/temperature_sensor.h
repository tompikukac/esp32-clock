#pragma once
#include <Arduino.h>
#include <cmath>

struct TemperatureData {
  float temperature;
  float humidity;
  float pressure;
  bool isValid() const {
    return !isnan(temperature) || !isnan(humidity) || !isnan(pressure);
  }
};

class TemperatureSensor {
public:
  virtual ~TemperatureSensor() = default;

  bool begin();
  bool isStarted() const;
  TemperatureData read();

protected:
  virtual bool beginImpl() = 0;
  virtual TemperatureData readImpl() = 0;

private:
  bool started = false;
};

#pragma once
#include <Arduino.h>

struct TemperatureData {
  float temperature;
  float humidity;
  bool valid;
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

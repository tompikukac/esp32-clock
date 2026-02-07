#include "temperature_sensor.h"

bool TemperatureSensor::begin() {
  started = beginImpl();
  return started;
}

bool TemperatureSensor::isStarted() const {
  return started;
}

TemperatureData TemperatureSensor::read() {
  if (!started) {
    return {};
  }
  return readImpl();
}

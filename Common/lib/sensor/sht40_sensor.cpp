#include "sht40_sensor.h"

SHT40Sensor::SHT40Sensor(TwoWire& wire)
  : wire(wire) {}

bool SHT40Sensor::beginImpl() {
  return sht.begin(&wire);
}

TemperatureData SHT40Sensor::readImpl() {
  TemperatureData data;
  sensors_event_t temp, humidity;
  sht.getEvent(&humidity, &temp);

  data.temperature = temp.temperature;
  data.humidity = humidity.relative_humidity;
  return data;
}

#include "sht30_sensor.h"

SHT30Sensor::SHT30Sensor(TwoWire& wire, uint8_t address)
  : wire(wire), address(address) {}

bool SHT30Sensor::beginImpl() {
  wire.begin(5, 6);  // SDA=GPIO5, SCL=GPIO6
  return sht.begin(address);
}

TemperatureData SHT30Sensor::readImpl() {
  TemperatureData data;
  data.temperature = sht.readTemperature();
  data.humidity = sht.readHumidity();
  data.valid = true;
  return data;
}

#pragma once
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include "temperature_sensor.h" 

class SHT30Sensor : public TemperatureSensor {
public:
  explicit SHT30Sensor(TwoWire& wire, uint8_t address = 0x44);

protected:
  bool beginImpl() override;
  TemperatureData readImpl() override;

private:
  TwoWire& wire;
  uint8_t address;
  Adafruit_SHT31 sht;
};

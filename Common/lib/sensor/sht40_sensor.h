#pragma once
#include <Wire.h>
#include <Adafruit_SHT4x.h>
#include "temperature_sensor.h"  // include header, not .cpp

class SHT40Sensor : public TemperatureSensor {
public:
  explicit SHT40Sensor(TwoWire& wire);

protected:
  bool beginImpl() override;
  TemperatureData readImpl() override;

private:
  TwoWire& wire;
  Adafruit_SHT4x sht;
};

#pragma once

#include "temperature_sensor.h"
#include "sht30_sensor.h"
#include "sht40_sensor.h"
#include <Wire.h>

enum class SensorType {
  SHT30,
  SHT40
};

TemperatureSensor* createSensor(SensorType type, TwoWire& wire);

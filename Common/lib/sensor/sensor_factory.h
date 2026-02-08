#pragma once

#include "temperature_sensor.h"
#include "sht30_sensor.h"
#include "sht40_sensor.h"
#include <Wire.h>
#include "sensor/sensor_type.h"

TemperatureSensor* createSensor(SensorType type, TwoWire& wire);

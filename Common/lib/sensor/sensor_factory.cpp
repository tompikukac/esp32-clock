#include "sensor_factory.h"

TemperatureSensor* createSensor(SensorType type, TwoWire& wire) {
  switch (type) {
    case SensorType::SHT30:
      return new SHT30Sensor(wire);
    case SensorType::SHT40:
      return new SHT40Sensor(wire);
    default:
      return nullptr;
  }
}

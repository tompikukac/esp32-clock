#pragma once
#include <ArduinoJson.h>
#include <IPAddress.h>
#include "sensor/sensor_type.h"

class ConfigData {
public:
    String name;
    unsigned long deepSleepTimeInSec;
    IPAddress ip;
    SensorType sensorType;
    bool oled;

    ConfigData();

    bool parse(const String& jsonStr);
    SensorType sensorTypeFromString(const char* str);
    String toString() const;
    bool hasStaticIP() const;
};

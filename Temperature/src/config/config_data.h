#pragma once
#include <ArduinoJson.h>
#include <IPAddress.h>

class ConfigData {
public:
    String name;
    unsigned long deepSleepTimeInSec;
    IPAddress ip;

    ConfigData();

    bool parse(const String& jsonStr);
    String toString() const;
    bool hasStaticIP() const;
};

#pragma once
#include <ArduinoJson.h>

class Config {
public:
    String name;
    unsigned long deepSleepTimeInSec = 599; 

    Config() : name("default-device") {}

    bool parse(const String& jsonStr) {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, jsonStr);
        if (error) {
            return false;
        }

        if (doc.containsKey("name")) {
            name = String(doc["name"].as<const char*>());
        }

        if (doc.containsKey("deepSleepTimeInSec")) {
            deepSleepTimeInSec = doc["deepSleepTimeInSec"].as<unsigned long>();
        }
        return true;
    }

    String toString() const {
        return "name: " + name + ", deepSleepTimeInSec: " + String(deepSleepTimeInSec);
    }
};

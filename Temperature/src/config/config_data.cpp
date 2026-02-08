#include "config_data.h"
#include "sensor/sensor_type.h"

ConfigData::ConfigData()
    : name("default-device"), deepSleepTimeInSec(599), ip(0, 0, 0, 0), sensorType(SensorType::SHT30), oled(false) {}

bool ConfigData::parse(const String& jsonStr) {
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

    if (doc.containsKey("ip")) {
        Serial.println("Parsing IP: " + String(doc["ip"].as<const char*>()));
        ip.fromString(doc["ip"].as<const char*>());
    }

    if (doc.containsKey("sensorType")) {
        const char* s = doc["sensorType"].as<const char*>();
        sensorType = sensorTypeFromString(s);
    }

    if (doc.containsKey("Oled")) {
        oled = doc["Oled"].as<bool>();
    }

    return true;
}

SensorType ConfigData::sensorTypeFromString(const char* str) {
    if (strcmp(str, "SHT30") == 0) return SensorType::SHT30;
    if (strcmp(str, "SHT40") == 0) return SensorType::SHT40;
    if (strcmp(str, "BME280") == 0) return SensorType::BME280;
    return SensorType::Unknown;
}

String ConfigData::toString() const {
    return "name: " + name + 
    ", deepSleepTimeInSec: " + String(deepSleepTimeInSec) + 
    ", ip: " + ip.toString()+ 
    ", sensorType: " + SensorTypeHelper::toString(sensorType)+ 
    ", Oled: " + String(oled ? "true" : "false");
}

bool ConfigData::hasStaticIP() const {
    return ip != IPAddress(0,0,0,0);
}

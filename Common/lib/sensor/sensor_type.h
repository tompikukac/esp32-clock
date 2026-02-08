#pragma once

enum class SensorType {
  SHT30,
  SHT40,
  BME280,
  Unknown
};

struct SensorTypeHelper {
    static const char* toString(SensorType type) {
        switch(type) {
            case SensorType::SHT30: return "SHT30";
            case SensorType::SHT40: return "SHT40";
            case SensorType::BME280: return "BME280";
            default: return "Unknown";
        }
    }
};
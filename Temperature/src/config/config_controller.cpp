#include "config_controller.h"
#include <Arduino.h>

ConfigController::ConfigController(const String& deviceId, const String& configRoot)
    : deviceId(deviceId), configRoot(configRoot) {}

ConfigData& ConfigController::load(bool forceLoad) {
    static ConfigData config;
    storage.begin();
    if (!storage.loadConfig(config) || forceLoad) {
        Serial.println("Reading default configuration...");
        String defaultJson = net.getConfig(configRoot + "default.json");
        config.parse(defaultJson);

        Serial.println("Reading specific configuration...");
        String configJson = net.getConfig(configRoot + deviceId + ".json");
        Serial.println("Config JSON: " + configJson);
        config.parse(configJson);

        if (config.name != "default-device") {
            Serial.println("Saving configuration...");
            storage.saveConfig(config);
        }
    } else {
        Serial.println("Configuration loaded from storage.");
    }
    storage.end();
    return config;
}

#include "config_controller.h"
#include <Arduino.h>
#include "logger.h"

ConfigController::ConfigController(const String& deviceId, const String& configRoot)
    : deviceId(deviceId), configRoot(configRoot) {}

ConfigData* ConfigController::load() {
    static ConfigData* config;
    storage.begin();
    config = storage.loadConfig();

    logger.println("Configuration loaded from storage: " + (config != nullptr ? config->toString() : "null"));
    storage.end();
    return config;
}


ConfigData& ConfigController::forceLoad() {
    static ConfigData config;
    storage.begin();

    logger.println("Reading default configuration...");
    String* defaultJson = net.getConfig(configRoot + "default.json");
    if (defaultJson != nullptr) {
        config.parse(*defaultJson);
    } else {
        logger.println("Failed to load default configuration!");
    }

    logger.println("Reading specific configuration...");
    String* configJson = net.getConfig(configRoot + deviceId + ".json");
    if (configJson != nullptr) {
        logger.println("Config JSON: " + *configJson);
        config.parse(*configJson);
    } else {
        logger.println("No specific configuration found for device " + deviceId);
    }

    if (config.name != "default-device") {
        logger.println("Saving configuration...");
        storage.saveConfig(config);
    }

    storage.end();
    return config;
}
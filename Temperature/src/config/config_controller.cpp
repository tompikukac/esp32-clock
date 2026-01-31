#include "config_controller.h"
#include <Arduino.h>
#include "logger.h"

ConfigController::ConfigController(const String& deviceId, const String& configRoot)
    : deviceId(deviceId), configRoot(configRoot) {}

ConfigData& ConfigController::load(bool forceLoad) {
    static ConfigData config;
    storage.begin();
    if (!storage.loadConfig(config) || forceLoad) {
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
    } else {
        logger.println("Configuration loaded from storage.");
    }
    storage.end();
    return config;
}

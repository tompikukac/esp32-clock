#include "config_storage.h"

void ConfigStorage::begin() {
    preferences.begin("config", false);
}

void ConfigStorage::saveConfig(const ConfigData& cfg) {
    preferences.putString("name", cfg.name);
    preferences.putULong("sleepInSec", cfg.deepSleepTimeInSec);
    preferences.putString("ip", cfg.ip.toString());
}

ConfigData* ConfigStorage::loadConfig() {
    if (!preferences.isKey("name")) {
        return nullptr;
    }
    String name = preferences.getString("name", "default-device");
    if (name == "default-device") {
        return nullptr;
    }

    ConfigData* cfg = new ConfigData();
    cfg->name = name;
    cfg->deepSleepTimeInSec =  preferences.getULong("sleepInSec", cfg->deepSleepTimeInSec);

    String ipStr = preferences.getString("ip", "");
    if (ipStr.length() > 0) {
        cfg->ip.fromString(ipStr);
    } else {
        cfg->ip = IPAddress(0,0,0,0);
    }

    return cfg;
}

void ConfigStorage::end() {
    preferences.end();
}

#include <Preferences.h>
#include "config.cpp"

class ConfigStorage {
    Preferences preferences;

public:
    void begin() {
        preferences.begin("config", false);
    }

    void saveConfig(const Config& cfg) {
        preferences.putString("name", cfg.name);
        preferences.putULong("sleepInSec", cfg.deepSleepTimeInSec);
    }

    bool loadConfig(Config& cfg) {
        if (!preferences.isKey("name")) {
            return false; // no saved config
        }
        String name = preferences.getString("name", cfg.name);
        if (name == "default-device") {
            return false; // default config, treat as no saved config
        }
        cfg.name = name;
        cfg.deepSleepTimeInSec = preferences.getULong("sleepInSec", cfg.deepSleepTimeInSec);
        return true;
    }

    void end() {
        preferences.end();
    }
};

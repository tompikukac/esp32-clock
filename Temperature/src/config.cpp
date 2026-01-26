#include <ArduinoJson.h>

class Config {
public:
    String name;

    Config() : name("default-device") {}  // default érték

    bool parse(const String& jsonStr) {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, jsonStr);
        if (error) {
            return false;
        }

        if (doc.containsKey("name")) {
            name = String(doc["name"].as<const char*>());
        }
        return true;
    }
};

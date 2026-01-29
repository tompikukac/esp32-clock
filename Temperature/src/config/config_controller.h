#pragma once

#include "config_data.h"
#include "config_storage.h"
#include "internet/internet_services.cpp"

class ConfigController {
    ConfigStorage storage;
    InternetServices net;
    String deviceId;
    String configRoot;
public:
    ConfigController(const String& deviceId, const String& configRoot);
    ConfigData& load(bool forceLoad);
};

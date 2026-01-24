#pragma once
#include <WiFi.h>
#include "wifi_config.h"

class WifiController {
public:
    WifiController();
    bool connect();
    void disconnect();
    bool isConnected() const;
};

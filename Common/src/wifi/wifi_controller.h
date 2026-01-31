#pragma once
#include <WiFi.h>
#include "wifi_config.h"

class WifiController {
public:
    WifiController();
    bool connect(const IPAddress& ip = IPAddress(0,0,0,0));
    void disconnect();
    bool isConnected() const;
    String getDeviceId();
};

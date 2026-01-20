#include "WifiController.h"

WifiController::WifiController() {
    WiFi.mode(WIFI_STA);
}

bool WifiController::connect() {
    Serial.print("Connecting to WiFi...");  
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int retries = 0;
    const int maxRetries = 20;
    
    while (WiFi.status() != WL_CONNECTED && retries < maxRetries) {
        delay(500);
        retries++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected. IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Not connected.");
    }
    return WiFi.status() == WL_CONNECTED;
}

void WifiController::disconnect() {
    WiFi.disconnect(true);   // drop connection + erase old settings
}

bool WifiController::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

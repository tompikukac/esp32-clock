#include "wifi_controller.h"

WifiController::WifiController() {
    WiFi.mode(WIFI_STA);
}

bool WifiController::connect() {
    Serial.println("WiFi: starting connection");

    // Hard reset WiFi state
    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_OFF);
    delay(100);

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);        // VERY important for stability
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    const uint32_t timeoutMs = 15000;
    uint32_t start = millis();

    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > timeoutMs) {
            Serial.println("WiFi: connection timeout");
            return false;
        }
        delay(250);
        yield(); // lets WiFi stack breathe
    }

    Serial.print("WiFi connected, IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("MAC: "+getDeviceId());
    return true;
}

void WifiController::disconnect() {
    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_OFF);   // <-- this is the key
    delay(100);
}

bool WifiController::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

String WifiController::getDeviceId() {
    String mac = WiFi.macAddress();
    mac.toUpperCase();
    mac.replace(":", "");
    return mac;
}
#include "wifi_controller.h"
#include "logger.h"

WifiController::WifiController() {
    WiFi.mode(WIFI_STA);
}

bool WifiController::connect(const IPAddress* ip) {
    logger.println("WiFi: starting connection");

    // Hard reset WiFi state
    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_OFF);
    delay(100);

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);        // VERY important for stability
    if (ip != nullptr) {
        logger.println("WiFi: setting static IP " + ip->toString());
        IPAddress local_IP = *ip; // from parameter
        IPAddress gateway(192,168,1,1);  // set your gateway
        IPAddress subnet(255,255,255,0); // set your subnet mask
        
        WiFi.config(local_IP, gateway, subnet);
    }
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    const uint32_t timeoutMs = 15000;
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > timeoutMs) {
            logger.println("WiFi: connection timeout");
            return false;
        }
        delay(250);
        yield(); // lets WiFi stack breathe
    }
    logger.print("WiFi connected, IP: ");
    logger.println(WiFi.localIP().toString());
    logger.println("MAC: "+getDeviceId());
    return true;
}

void WifiController::disconnect() {
    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_OFF);
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
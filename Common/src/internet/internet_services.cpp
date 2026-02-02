#include <Arduino.h>
#include <time.h>
#include <HTTPClient.h>
#include "logger.h"

class InternetServices {
public:
    InternetServices() {

    }

    struct tm getTime() {
        // Set timezone and NTP server
        configTime(0, 0, "pool.ntp.org");  
        // For CET/CEST (Hungary):
        setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
        tzset();

        // Wait for time to be set
        struct tm timeinfo;
        int retries = 0;
        const int maxRetries = 20;

        while (!getLocalTime(&timeinfo)) {
            logger.println("Waiting for time...");
            delay(500);
            retries++;

            if (retries >= maxRetries) {
                logger.println("Failed to get time after attempts, exiting.");
                break;   // or return; depending on your design
            }
        }

        logger.println(&timeinfo, "Time: %Y-%m-%d %H:%M:%S");
        return timeinfo;
    }

    String* getConfig(const String& url) {
    static String payload;

    for (int i = 0; i < 5; i++) {
        HTTPClient http;
        http.begin(url);

        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            payload = http.getString();
            http.end();

            if (payload.length() > 0) {
                return &payload;
            }
        } else {
            http.end();
        }

        delay(2000);
    }

    return nullptr;
    }
};

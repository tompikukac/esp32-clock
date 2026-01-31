#include <Arduino.h>
#include "deepsleep.h"
#include "esp_wifi.h"
#include "logger.h"

DeepSleep::DeepSleep() {

}
void DeepSleep::sleepInSec(uint32_t seconds) {
    uint64_t sleepTimeUs = (uint64_t)seconds * 1000000ULL;
 
    if (Serial) {
        logger.printf("Entering deep sleep for %u seconds...\n", seconds);
        logger.println("Closing Serial...");
        logger.end();
        delay(50); 
        
    }
    esp_wifi_stop();
    delay(200); // Short buffer for power stabilization
    
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    esp_sleep_enable_timer_wakeup(sleepTimeUs);
    delay(100);
    esp_deep_sleep_start();
}

void DeepSleep::sleep(uint32_t minutes, uint32_t seconds) {
    const uint32_t totalSeconds = minutes * 60 + seconds;
    sleepInSec(totalSeconds);
}

void DeepSleep::lightSleepInSec(uint32_t seconds) {
    uint64_t sleepTimeUs = static_cast<uint64_t>(seconds) * 1000000ULL;

    logger.printf("Entering light sleep for %u seconds...\n", seconds);
    logger.end();
    delay(100);

    esp_sleep_enable_timer_wakeup(sleepTimeUs);

    esp_light_sleep_start();

    logger.println("Woke up from light sleep!");
}
#include <MD_Parola.h>
#include <MD_MAX72XX.h>

class ParolaDisplay {
    #define HARDWARE_TYPE MD_MAX72XX::FC16_HW
public:
    ParolaDisplay(
        // md_parola_hw_type_t hardwareType,
        uint8_t dataPin,
        uint8_t clkPin,
        uint8_t csPin,
        uint8_t maxDevices
    )
    : display(HARDWARE_TYPE, dataPin, clkPin, csPin, maxDevices)
    {}

    void begin() {
        display.begin();
    }

    void setText(const char* msg) {
        strncpy(storedMsg, msg, sizeof(storedMsg));
        storedMsg[sizeof(storedMsg) - 1] = '\0';
        display.displayText(storedMsg, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    }

    void setBrightness(uint8_t level) {
        // MD_Parola accepts values 0–15
        if (level > 15) level = 15;
        display.setIntensity(level);
    }

    void update() {
        display.displayAnimate();
    }

private:
    MD_Parola display;
    char storedMsg[128];
};
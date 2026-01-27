#include <Adafruit_NeoPixel.h>

struct LedColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
namespace Colors {
    const LedColor White   = {255, 255, 255};
    const LedColor Red     = {255, 0, 0};
    const LedColor Lime    = {0, 255, 0};
    const LedColor Blue    = {0, 0, 255};
    const LedColor Yellow  = {255, 255, 0};
    const LedColor Cyan    = {0, 255, 255};
    const LedColor Magenta = {255, 0, 255};
    const LedColor Silver  = {192, 192, 192};
    const LedColor Gray    = {128, 128, 128};
    const LedColor Maroon  = {128, 0, 0};
    const LedColor Olive   = {128, 128, 0};
    const LedColor Green   = {0, 128, 0};
    const LedColor Purple  = {128, 0, 128};
    const LedColor Teal    = {0, 128, 128};
    const LedColor Navy    = {0, 0, 128};
    const LedColor Black   = {0, 0, 0};
}

class Esp32C3ZeroLed {
private:
    Adafruit_NeoPixel _pixel;
    uint8_t _pin;
    uint8_t _brightness;

public:
    Esp32C3ZeroLed(uint8_t pin = 10, uint8_t brightness = 40) 
        : _pixel(1, pin, NEO_GRB + NEO_KHZ800), _pin(pin), _brightness(brightness) {}

    void begin() {
        _pixel.begin();
        _pixel.setBrightness(_brightness);
        _pixel.show();
    }

    void setBrightness(uint8_t level) {
        _brightness = level;
        _pixel.setBrightness(_brightness);
        _pixel.show();
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b) {
        _pixel.setPixelColor(0, _pixel.Color(r, g, b));
        _pixel.show();
    }

    void setColor(LedColor color) {
        _pixel.setPixelColor(0, _pixel.Color(color.r, color.g, color.b));
        _pixel.show();
    }

    void off() {
        setColor(0, 0, 0);
    }

    void setHue(uint16_t hue) {
        uint32_t rgbcolor = _pixel.ColorHSV(hue, 255, 255);
        _pixel.setPixelColor(0, rgbcolor);
        _pixel.show();
    }
};
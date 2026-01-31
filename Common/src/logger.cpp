#include "logger.h"
#include <cstdarg>

Logger logger;

void Logger::begin(uint32_t baud, uint32_t waitMs) {
  Serial.begin(baud);

  unsigned long start = millis();
  while (!Serial && millis() - start < waitMs) {
    delay(10);
  }

  _enabled = Serial;
}

void Logger::print(const String& s) {
  if (!_enabled) return;
  Serial.print(s);
}

void Logger::println(const String& s) {
  if (!_enabled) return;
  Serial.println(s);
}

void Logger::println(const struct tm* timeinfo, const char* format) {
    if (!_enabled || Serial.availableForWrite() < 32) return;

    char buf[64];
    if (strftime(buf, sizeof(buf), format, timeinfo) > 0) {
        Serial.println(buf);
    }
}

void Logger::printf(const char* fmt, ...) {
    if (!_enabled) return;
    va_list args;
    va_start(args, fmt);
    
    char* buf = nullptr;
    if (vasprintf(&buf, fmt, args) >= 0 && buf != nullptr) {
        Serial.print(buf);
        free(buf);
    }
    
    va_end(args);
}

void Logger::end() {
  if (!_enabled) return;
  Serial.flush();
  delay(20);
  // Serial.end(); // Shut down the UART hardware entirely. On some boards this can interfere with USB‑serial bridges or cause the chip to hang briefly.
  _enabled = false;
}

bool Logger::enabled() const {
  return _enabled;
}

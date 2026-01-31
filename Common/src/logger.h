#pragma once
#include <Arduino.h>

class Logger {
public:
  void begin(uint32_t baud = 115200, uint32_t waitMs = 500);

  void print(const String& s);
  void println(const String& s);
  void printf(const char* fmt, ...);
  void println(const struct tm* timeinfo, const char* format);

  void end();
  bool enabled() const;

private:
  bool _enabled = false;
};

extern Logger logger;

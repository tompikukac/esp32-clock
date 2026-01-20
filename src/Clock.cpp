#include <Arduino.h>
#include <time.h>

class Clock {
public:
    Clock() : _baseTime(0), _setAtMillis(0) {}

    void setTime(const struct tm& t) {
        _baseTime = mktime((struct tm*)&t);   // convert to epoch seconds
        _setAtMillis = millis();
    }

    struct tm getTime() const {
        time_t now = _baseTime + (millis() - _setAtMillis) / 1000;
        struct tm result;
        localtime_r(&now, &result);
        return result;
    }

    String toString() const {
        struct tm now = getTime();
        char buf[9];  // "HH:MM:SS"
        sprintf(buf, "%02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
        return String(buf);
    }

private:
    time_t _baseTime;        // stored as epoch seconds
    unsigned long _setAtMillis;
};

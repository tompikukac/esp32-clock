#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include "bme280_sensor.h"
#include "logger.h"

class InfluxController {
public:
  InfluxController(const char* host, const char* org, const char* bucket, const char* token)
    : _host(host), _org(org), _bucket(bucket), _token(token) {}

  void send(const BME280Data& data, const String& name) {
    if (WiFi.status() != WL_CONNECTED) {
      logger.println("WiFi not connected");
      return;
    }

    HTTPClient http;
    String url = String(_host) + "/api/v2/write?org=" + _org + "&bucket=" + _bucket + "&precision=s";
    http.begin(url);
    http.addHeader("Authorization", String("Token ") + _token);
    http.addHeader("Content-Type", "text/plain");

    String payload = buildPayload(data, name);

    int code = http.POST(payload);
    if (code > 0) {
      logger.printf("InfluxDB response: %d\n", code);
    } else {
      logger.printf("InfluxDB POST error: %s\n", http.errorToString(code).c_str());
    }
    http.end();
  }

private:
  const char* _host;
  const char* _org;
  const char* _bucket;
  const char* _token;

  String buildPayload(const BME280Data& data, const String& name) {
    return "environment,name=" + name +
          " temperature=" + String(data.temperature, 2) +
          ",humidity=" + String(data.humidity, 2) +
          ",pressure=" + String(data.pressure, 2);
  }
};

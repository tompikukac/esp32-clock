#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include "logger.h"
#include "sensor/temperature_sensor.h"

class InfluxController {
public:
  InfluxController(const char* host, const char* org, const char* bucket, const char* token)
    : _host(host), _org(org), _bucket(bucket), _token(token) {}

  boolean send(const TemperatureData& data, const String& name) {
    if (WiFi.status() != WL_CONNECTED) {
      logger.println("WiFi not connected");
      return false;
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
      http.end();
      return true;
    } else {
      logger.printf("InfluxDB POST error: %s\n", http.errorToString(code).c_str());
      http.end();
      return false;
    }
    
  }

  boolean sendLog(const String& message, const String& name) {
    if (WiFi.status() != WL_CONNECTED) {
      logger.println("WiFi not connected");
      return false;
    }

    logger.println(message);

    HTTPClient http;
    String url = String(_host) + "/api/v2/write?org=" + _org + "&bucket=esp32logs&precision=s";

    http.begin(url);
    http.addHeader("Authorization", String("Token ") + _token);
    http.addHeader("Content-Type", "text/plain");

    String payload = buildLogPayload(message, name);

    int code = http.POST(payload);
    if (code > 0) {
      logger.printf("InfluxDB log response: %d\n", code);
      http.end();
      return true;
    } else {
      logger.printf("InfluxDB POST error: %s\n", http.errorToString(code).c_str());
      http.end();
      return false;
    }
}


private:
  const char* _host;
  const char* _org;
  const char* _bucket;
  const char* _token;

String buildPayload(const TemperatureData& data, const String& name) {
    String payload = "environment,name=" + name;
    bool firstField = true;

if (!isnan(data.temperature)) {
    payload += (firstField ? " " : ",");
    payload += "temperature=" + String(data.temperature, 2);
    firstField = false;
}

    if (!isnan(data.humidity)) {
        payload += (firstField ? " " : ",");
        payload += "humidity=" + String(data.humidity, 2);
        firstField = false;
    }

    if (!isnan(data.pressure)) {
        payload += (firstField ? " " : ",");
        payload += "pressure=" + String(data.pressure, 2);
        firstField = false;
    }

    return payload;
}

  String buildLogPayload(const String& message, const String& name) {
    String safe = message;
    safe.replace("\"", "\\\""); // escape quotes

    return "logs,name=" + name + " message=\"" + safe + "\"";
  } 

};

#include <Arduino.h>
// #include <time.h>

// #include <MD_Parola.h>
// #include <MD_MAX72XX.h>
// #include <SPI.h>
#include "wifi/wifi_controller.h"
#include "internet/internet_services.cpp"
#include "clock/clock.cpp"
#include "deepsleep.cpp"
#include "display/parola_display.cpp"


#define MAX_DEVICES 4
#define DATA_PIN  23   // DIN
#define CLK_PIN   18   // CLK
#define CS_PIN    5    // CS
ParolaDisplay myDisplay(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


#define LED 2
InternetServices net;
Clock clockObj;
WifiController* wifi;
DeepSleep deepSleep;

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  Serial.println("WiFi connecting...");
  wifi = new WifiController();

  if (wifi->connect()) {
    Serial.println("WiFi connected!");
    Serial.println(WiFi.localIP());

    clockObj.setTime(net.getTime());
    Serial.println(clockObj.toStringHMS());


    myDisplay.begin();
    myDisplay.setBrightness(1);

  } else {
      Serial.println("WiFi connection failed");
      deepSleep.sleep(0,5);
  }
}

void loop() {
  delay(500);
  digitalWrite(LED,HIGH);
  // myDisplay.setText(clockObj.toStringHM(' ').c_str());
  // myDisplay.update();
  delay(500);
  digitalWrite(LED,LOW);
  Serial.println(clockObj.toStringHMS());
  Serial.println(clockObj.toStringHM());

  myDisplay.setText(clockObj.toStringHM().c_str());
  myDisplay.update();

  // deepSleep.sleep(0,20);
}
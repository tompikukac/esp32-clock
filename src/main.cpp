#include <Arduino.h>
// #include <time.h>

// #include <MD_Parola.h>
// #include <MD_MAX72XX.h>
// #include <SPI.h>
#include "WifiController.h"
#include "InternetServices.cpp"
#include "Clock.cpp"

WifiController* wifi;

// #define HARDWARE_TYPE MD_MAX72XX::FC16_HW
// #define MAX_DEVICES 4
// #define DATA_PIN  23   // DIN
// #define CLK_PIN   18   // CLK
// #define CS_PIN    5    // CS

// MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
// // MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// // MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// #define LED 2

// // void connectToWiFi() {
// //     WiFi.mode(WIFI_STA);
// //     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

// //     Serial.print("Connecting...");
// //     int retry = 0;

// //     while (WiFi.status() != WL_CONNECTED) {
// //         delay(500);
// //         Serial.print(".");
// //         retry++;

// //         if (retry > 40) {          // kb. 20 másodperc
// //             Serial.println(" retry");
// //             WiFi.disconnect();
// //             WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
// //             retry = 0;
// //         }
// //     }

// //     Serial.println("\nWiFi connected");
// //     Serial.print("IP add: ");
// //     Serial.println(WiFi.localIP());
// // }

// struct tm getTime() {
//   // Set timezone and NTP server
//   configTime(0, 0, "pool.ntp.org");  
//   // For CET/CEST (Hungary):
//   setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
//   tzset();

//   // Wait for time to be set
//   struct tm timeinfo;
//   int retries = 0;
//   const int maxRetries = 20;

//   while (!getLocalTime(&timeinfo)) {
//       Serial.println("Waiting for time...");
//       delay(500);
//       retries++;

//       if (retries >= maxRetries) {
//           Serial.println("Failed to get time after attempts, exiting.");
//           break;   // or return; depending on your design
//       }
//   }

//   Serial.println(&timeinfo, "Time: %Y-%m-%d %H:%M:%S");
//   return timeinfo;
// }

// void deepSleep() {
//   // Sleep duration
//   const uint32_t minutes = 1;
//   const uint32_t seconds = minutes * 60;
//   const uint64_t sleepTimeUs = (uint64_t)seconds * 1000000ULL;

//   // Human‑readable message
//   Serial.printf("Entering deep sleep for %u minutes (%u seconds)...\n",
//                 minutes, seconds);
//   Serial.flush();

//   esp_sleep_enable_timer_wakeup(sleepTimeUs);
//   esp_deep_sleep_start();
// }

// char timeStr[16];
// void showTime() {
//   struct tm now = getTime();

//   // sprintf(timeStr, "%02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
//   sprintf(timeStr, "%02d:%02d", now.tm_hour, now.tm_min);
//   // sprintf(timeStr, "aaa");
//   Serial.println(timeStr);

//   Serial.println("LED...");
//   display.begin();
//   display.displayClear();
//   // display.displayText("Hello", PA_CENTER, 50, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
//   // display.displayText(timeStr, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
 
//   display.displayText(timeStr, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);

//   // mx.begin();
//   // mx.clear();
//   // mx.control(MD_MAX72XX::INTENSITY, 5);
//   // for (uint8_t device = 0; device < MAX_DEVICES; device++) {
//   //   for (uint8_t row = 0; row < 8; row++) {
//   //     for (uint8_t col = 0; col < 8; col++) {
//   //       bool state = (col % 2 == 0);
//   //       mx.setPoint(row, col, state);
//   //     }
//   //   }
//   // }
//   // mx.setPoint(0, 0, true);   // row 0, col 0
// }

// void setup() {
//   // Set pin mode
//   pinMode(LED,OUTPUT);
//   Serial.begin(115200);
//   if (wifi.connect()) {
//         Serial.println("WiFi connected!");
//         Serial.println(WiFi.localIP());
//     } else {
//         Serial.println("WiFi connection failed");
//     }
//   // getTime();
//   showTime();
// }


// void loop() {
//   // showTime();
//   display.displayAnimate();
//   delay(1500);
// // you can set the delay time by adjusting the parameter of delay();
//   // digitalWrite(LED,HIGH);
//   // delay(500);
//   // digitalWrite(LED,LOW);
//   Serial.println("aaa");
//   // deepSleep();
// }


#define LED 2
InternetServices net;
Clock clockObj;

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  Serial.println("WiFi connecting...");
  wifi = new WifiController();

  if (wifi->connect()) {
      Serial.println("WiFi connected!");
      Serial.println(WiFi.localIP());

      clockObj.setTime(net.getTime());
      Serial.println(clockObj.toString());

  } else {
      Serial.println("WiFi connection failed");
  }
}

void loop() {
  delay(500);
  digitalWrite(LED,HIGH);
  delay(500);
  digitalWrite(LED,LOW);
  Serial.println("aaa");
  Serial.println(clockObj.toString());
}
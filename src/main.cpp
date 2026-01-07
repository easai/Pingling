#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>
#include <HTTPClient.h>
#include "config.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char ssid[] = SECRET_SSID;
char password[] = SECRET_PASS;
char url[] = SITE_URL;

unsigned long lastUpdate = 0;
const unsigned long interval = 60000; // 1 minute

bool isUp(const char* url) {
    HTTPClient http;
    http.setTimeout(5000);
    http.begin(url);
    int code = http.GET();
    http.end();
    return (code > 0 && code < 500);
}

void updateStatus() {
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0/2", 1);
    tzset();

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("NTP failed");
        return;
    }

    bool isDST = timeinfo.tm_isdst > 0;
    const char* tzName = isDST ? "EDT" : "EST";

    char dateStr[32];
    strftime(dateStr, sizeof(dateStr), "      %Y-%m-%d", &timeinfo);
    char timeStr[32];
    strftime(timeStr, sizeof(timeStr), "   %H:%M", &timeinfo);

    bool ok = isUp(url);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);

    display.print(dateStr);
    display.println("");
    display.setTextSize(2);
    display.print(timeStr);
    display.println("");

    display.setTextSize(1);
    display.println(url);
    display.println("");

    display.setTextSize(3);
    display.println(ok ? "   UP" : " DOWN");

    display.display();
}


void setup() {
    Serial.begin(115200);
    delay(200);

    Wire.begin(8, 9);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Connecting WiFi...");
    display.display();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi OK");
    display.display();
    delay(500);

    // Set timezone BEFORE NTP
    setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1);
    // setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0/2", 1);
    tzset();

    // Start NTP
    configTime(0, 0, "pool.ntp.org");

    // *** WAIT FOR REAL NTP TIME ***
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        Serial.println("Waiting for NTP...");
        delay(500);
    }

    // Now the time is correct
    updateStatus();
}

void loop() {
    unsigned long now = millis();

    if (now - lastUpdate >= interval) {
        lastUpdate = now;
        updateStatus();
    }
}

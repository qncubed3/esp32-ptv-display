#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define STOP_ID "G1071"         // Flinders Street
#define PLATFORM "P1"           // Choose your platform
#define SSID "SSID";            // Wifi SSID here
#define PASSWORD "PASSWORD";    // Wifi password here

#define LIMIT 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LINE_HEIGHT 12

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const String url =
    String("https://esp32-ptv-display-um8x.vercel.app/departures?") + 
      "stop_id=" + STOP_ID + 
      "&platform=" + PLATFORM + 
      "&limit=" + String(LIMIT);

void setup() {
    Serial.begin(115200);

    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("failed to start SSD1306 OLED"));
        while (1);
    }

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.println("Connecting WiFi...");
    oled.display();

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected");

    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.println("WiFi Connected");
    oled.display();

    delay(1000);
}

void loop() {
    oled.setTextSize(1);

    HTTPClient http;
    http.begin(url);

    int httpCode = http.GET();

    if (httpCode <= 0) {
        oled.clearDisplay();
        oled.setCursor(0, 0);
        oled.println("HTTP Failed");
        oled.display();

        http.end();
        delay(30000);
        return;
    }

    String payload = http.getString();
    http.end();

    Serial.println(payload);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        oled.clearDisplay();
        oled.setCursor(0, 0);
        oled.println("JSON Failed");
        oled.display();

        delay(30000);
        return;
    }

    const char* station = doc["stop"]["name"];
    const char* platform = doc["stop"]["platform"];

    oled.clearDisplay();
    oled.setTextColor(WHITE);

    oled.setCursor(0, 0);
    oled.print(station);
    oled.print(" Platform ");
    oled.println(platform);

    JsonArray departures = doc["departures"];

    int y = 16;

    for (JsonObject dep : departures) {
        const char* dest = dep["dest"];
        const char* time = dep["time"];
        int remain = dep["remain"];

        String destShort = String(dest);

        if (destShort.length() > 8) {
            destShort = destShort.substring(0, 8);
        }

        oled.setCursor(0, y);

        oled.print(time);
        oled.print(" ");
        oled.print(destShort);
        oled.print(" (");
        oled.print(remain);
        oled.print("m)");

        y += LINE_HEIGHT;
    }

    oled.display();

    delay(30000);
}
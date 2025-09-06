// Blynk Authentication
#define BLYNK_TEMPLATE_ID "TMPL3tn4CGIpV"
#define BLYNK_TEMPLATE_NAME "Esp8266"
#define BLYNK_AUTH_TOKEN "EGamxiacJmo02kYDPmVj6bOBwmp0BbMQ"

#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi Credentials
char ssid[] = "Elvish Patel";
char pass[] = "elvish123patel";

// Virtual Pins for Blynk
#define VIRTUAL_TEMP V1
#define VIRTUAL_HUMIDITY V2
#define VIRTUAL_AIR_QUALITY V3
#define VIRTUAL_AIR_QUALITY_TEXT V4

// Define Software Serial (Tx = D5, Rx = D6)
SoftwareSerial nodemcu(D6, D5);

void setup() {
    Serial.begin(115200);
    nodemcu.begin(9600);  
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
    Blynk.run(); 

    if (nodemcu.available()) {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, nodemcu);

        if (error) {
            Serial.println("JSON Parsing Failed!");
            return;
        }

        JsonObject data = doc.as<JsonObject>();

        float temperature = data["temperature"];
        float humidity = data["humidity"];
        int airQualityIndex = data["air_quality"];

        Serial.print("Received -> Temp: "); Serial.print(temperature); Serial.print("°C, ");
        Serial.print("Humidity: "); Serial.print(humidity); Serial.print("%, ");
        Serial.print("AQI: "); Serial.println(airQualityIndex);

        // Convert AQI value to Text
        String airQualityText;
        if (airQualityIndex <= 50) airQualityText = "Good 😊";
        else if (airQualityIndex <= 100) airQualityText = "Moderate 😐";
        else if (airQualityIndex <= 150) airQualityText = "Unhealthy 😷";
        else airQualityText = "Hazardous ⚠️";

        // Send Data to Blynk App
        Blynk.virtualWrite(VIRTUAL_TEMP, temperature);
        Blynk.virtualWrite(VIRTUAL_HUMIDITY, humidity);
        Blynk.virtualWrite(VIRTUAL_AIR_QUALITY, airQualityIndex);
        Blynk.virtualWrite(VIRTUAL_AIR_QUALITY_TEXT, airQualityText);

        // Trigger Blynk Notification if AQI is Hazardous
        if (airQualityIndex >= 100) { 
            Blynk.logEvent("air_quality_alert", "⚠️ Air Quality is " + airQualityText + "! Take Precautions!");
        }
    }

    delay(500);
}

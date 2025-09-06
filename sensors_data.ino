#include <AM2302-Sensor.h>  
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// Define Pins
#define DHTPIN 7       // AM2302(DHT22) sensor pin
#define DHTTYPE DHT22  // Define AM2302 type
#define MQ135_PIN A0   // MQ-135 sensor pin

AM2302::AM2302_Sensor am2302{DHTPIN};
SoftwareSerial nodemcu(5, 6);

void setup() {
    Serial.begin(115200);
    nodemcu.begin(9600); 

    Serial.println(F("\n >>> Sensor System Initialized <<<\n"));

    if (am2302.begin()) {
        delay(3000);
    } else {
        Serial.println("Error: Sensor check failed! Check connections.");
        while (true) delay(10000);
    }
}

void loop() {
    // Read Temperature & Humidity
    auto status = am2302.read();
    float temperature = am2302.get_Temperature();
    float humidity = am2302.get_Humidity();

    // Read MQ-135 (Air Quality)
    int rawValue = analogRead(MQ135_PIN);
    float normalized_gas = map(rawValue, 0, 1023, 0, 300);  // Scale to AQI

    // Adjust AQI using Temperature & Humidity
    float temp_factor = (temperature > 35) ? (temperature - 35) * 0.5 : 0;
    float humidity_factor = (humidity > 60) ? (humidity - 60) * 0.3 : 0;

    // Compute Final AQI
    float airQualityIndex = normalized_gas + temp_factor + humidity_factor;
    airQualityIndex = constrain(airQualityIndex, 0, 300); // Ensure valid range

    // Create JSON Object
    DynamicJsonDocument doc(1024);
    JsonObject data = doc.to<JsonObject>();

    data["temperature"] = temperature;
    data["humidity"] = humidity;
    data["air_quality"] = airQualityIndex;

    // Send JSON to ESP8266
    serializeJson(data, nodemcu); 
    
    // Display in Serial Monitor
    Serial.print("Sent -> Temp: "); Serial.print(temperature); Serial.print("°C, ");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.print("%, ");
    Serial.print("AQI: "); Serial.println(airQualityIndex);

    doc.clear();  
    delay(1000);
}

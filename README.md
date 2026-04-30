<div align="center">

# 🌍 IoT-Based Air Quality Monitoring System

[![Open Source](https://img.shields.io/badge/Open%20Source-✔-brightgreen?style=for-the-badge)](https://github.com/)
[![IoT Enabled](https://img.shields.io/badge/IoT-Enabled-blue?style=for-the-badge&logo=arduino)](https://www.arduino.cc/)
[![Real Time](https://img.shields.io/badge/Real--Time-Monitoring-orange?style=for-the-badge)](https://blynk.io/)
[![License: MIT](https://img.shields.io/badge/License-MIT-purple?style=for-the-badge)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Arduino%20%7C%20ESP8266-teal?style=for-the-badge&logo=arduino)](https://www.arduino.cc/)
[![Cloud](https://img.shields.io/badge/Cloud-Blynk%20IoT-red?style=for-the-badge)](https://blynk.io/)

<br/>

**A low-cost, real-time environmental monitoring solution using Arduino UNO + NodeMCU ESP8266 to track AQI, temperature, and humidity — accessible from anywhere via smartphone.**

</div>

---

## 📸 Project Preview

**Hardware Setup**

![Hardware Setup](https://github.com/user-attachments/assets/5e4d882c-b7b4-436a-b9a9-3b3d738b1e14)

**Mobile Dashboard**

![Mobile Dashboard](https://github.com/user-attachments/assets/4f8ff33d-82e1-42b9-a7d9-a06d8d811a17)

---

## 📌 Project Overview

This project is an **IoT-enabled smart air quality monitoring system** that tracks **Air Quality Index (AQI)**, **temperature**, and **humidity** in real time. It uses the `MQ-135` gas sensor and the `AM2302 (DHT22)` sensor for environmental readings.

The **Arduino UNO** reads both sensors, computes a temperature- and humidity-adjusted AQI, serializes the result as **JSON**, and sends it to a **NodeMCU ESP8266** via SoftwareSerial. The ESP8266 parses the JSON and pushes all readings to the **Blynk IoT Cloud** over WiFi — displaying live values on a smartphone dashboard and triggering alerts when air quality degrades.

---

## 🛠 Hardware Used

| Component | Purpose |
|:----------|:--------|
| **Arduino UNO** | Reads sensors, computes AQI, sends JSON over SoftwareSerial |
| **NodeMCU ESP8266** | Receives JSON from Arduino, connects to WiFi, uploads to Blynk |
| **MQ-135 Gas Sensor** | Measures gas concentration (analog, pin `A0`) |
| **AM2302 / DHT22 Sensor** | Measures temperature & humidity (digital, pin `D7`) |
| **Jumper Wires & Breadboard** | Prototyping connections — no soldering required |
| **5V Power Supply** | Powers Arduino and NodeMCU |

---

## ⚙️ How It Works

```
┌─────────────┐   analogRead(A0)    ┌──────────────────────┐
│  MQ-135     │ ──────────────────► │                      │
│  Gas Sensor │                     │     Arduino UNO      │
└─────────────┘                     │                      │
                                    │  1. Read sensors     │
┌─────────────┐   Digital (D7)      │  2. Compute AQI      │
│  AM2302     │ ──────────────────► │  3. Serialize JSON   │
│  (DHT22)    │                     │                      │
└─────────────┘                     └──────────┬───────────┘
                                               │
                                    SoftwareSerial (D5/D6)
                                      JSON @ 9600 baud
                                               │
                                    ┌──────────▼───────────┐
                                    │   NodeMCU ESP8266    │
                                    │                      │
                                    │  1. Parse JSON       │
                                    │  2. Map AQI to text  │
                                    │  3. Write to Blynk   │
                                    │  4. Trigger alerts   │
                                    └──────────┬───────────┘
                                               │
                                           WiFi / HTTP
                                               │
                                    ┌──────────▼───────────┐
                                    │   Blynk IoT Cloud    │
                                    │   V1 → Temperature   │
                                    │   V2 → Humidity      │
                                    │   V3 → AQI (number)  │
                                    │   V4 → AQI (text)    │
                                    └──────────────────────┘
```

### Step-by-Step Flow

| Step | Stage | Description |
|:----:|:------|:------------|
| **1** | 🔍 **Sense** | Arduino reads MQ-135 via `analogRead(A0)` and AM2302 via the `AM2302-Sensor` library on pin `D7` |
| **2** | ⚙️ **Compute** | AQI is calculated from the raw gas value mapped to 0–300, then corrected using temperature and humidity factors |
| **3** | 📤 **Serialize** | Arduino packages all three values into a JSON object using `ArduinoJson` and sends it via SoftwareSerial at 9600 baud |
| **4** | 📥 **Parse** | NodeMCU deserializes the incoming JSON using `ArduinoJson` and extracts temperature, humidity, and AQI |
| **5** | ☁️ **Upload** | NodeMCU writes values to Blynk virtual pins V1–V4 and fires an alert event if AQI ≥ 100 |
| **6** | 📱 **Visualize** | Blynk app displays live readings and sends a push notification on poor air quality |

---

## 📊 AQI Calculation Logic

The AQI is computed in `sensor_data.ino` using three factors — raw gas reading plus environmental correction:

```c
// 1. Map raw MQ-135 analog value (0–1023) to a 0–300 scale
float normalized_gas = map(rawValue, 0, 1023, 0, 300);

// 2. Temperature correction factor — penalty applied above 35°C
float temp_factor = (temperature > 35) ? (temperature - 35) * 0.5 : 0;

// 3. Humidity correction factor — penalty applied above 60% RH
float humidity_factor = (humidity > 60) ? (humidity - 60) * 0.3 : 0;

// 4. Final AQI — constrained to valid 0–300 range
float airQualityIndex = normalized_gas + temp_factor + humidity_factor;
airQualityIndex = constrain(airQualityIndex, 0, 300);
```

---

## 📈 AQI Status Levels

The numeric AQI is mapped to a human-readable text label in `Wifi.ino` before being sent to Blynk on virtual pin `V4`:

| AQI Range | Status | Blynk Alert |
|:---------:|:------:|:-----------:|
| `0 – 50` | 🟢 Good 😊 | No |
| `51 – 100` | 🟡 Moderate 😐 | No |
| `101 – 150` | 🔴 Unhealthy 😷 | ✅ Yes |
| `151 – 300` | ⚫ Hazardous ⚠️ | ✅ Yes |

> Alert threshold: `airQualityIndex >= 100` triggers `Blynk.logEvent("air_quality_alert", ...)` as a push notification.

---

## 🔌 Wiring & Pin Configuration

> ⚠️ **Important:** Use a **voltage divider (1kΩ + 2kΩ)** on the Arduino TX → NodeMCU RX line to step 5V down to 3.3V and protect the ESP8266 GPIO pins.

### AM2302 (DHT22) → Arduino UNO

| AM2302 Pin | Arduino Pin | Notes |
|:----------:|:-----------:|:------|
| VCC | 5V | Power supply |
| GND | GND | Common ground |
| DATA | **D7** | `DHTPIN 7` in code — add 10kΩ pull-up to VCC |

### MQ-135 Gas Sensor → Arduino UNO

| MQ-135 Pin | Arduino Pin | Notes |
|:----------:|:-----------:|:------|
| VCC | 5V | Power supply |
| GND | GND | Common ground |
| A0 (Analog out) | **A0** | `MQ135_PIN A0` in code |

### Arduino UNO → NodeMCU ESP8266 (SoftwareSerial)

| Arduino Pin | NodeMCU Pin | Notes |
|:-----------:|:-----------:|:------|
| **D5** (TX) | **D6** (RX) | Arduino transmits → NodeMCU receives; use voltage divider here |
| **D6** (RX) | **D5** (TX) | NodeMCU transmits → Arduino receives |
| GND | GND | Common ground — required for serial to work |

> SoftwareSerial declarations in code:
> - `sensor_data.ino` (Arduino): `SoftwareSerial nodemcu(5, 6);` → RX on D5, TX on D6
> - `Wifi.ino` (NodeMCU): `SoftwareSerial nodemcu(D6, D5);` → RX on D6, TX on D5

### Blynk Virtual Pin Mapping

| Virtual Pin | Data | Data Type |
|:-----------:|:-----|:---------:|
| `V1` | Temperature (°C) | Float |
| `V2` | Humidity (%) | Float |
| `V3` | AQI (numeric 0–300) | Int |
| `V4` | AQI status text | String |

---

## ✨ Key Features

- ✅ **Real-Time Monitoring** — Sensors polled every 1 second; readings sent to Blynk with 500ms loop on NodeMCU
- ✅ **Multi-Factor AQI** — Accounts for raw gas concentration plus temperature and humidity correction
- ✅ **JSON Serial Protocol** — Structured `ArduinoJson` packets over SoftwareSerial at 9600 baud for reliable inter-device communication
- ✅ **Mobile Dashboard** — Blynk virtual pins V1–V4 drive live gauges, value displays, and a text status label
- ✅ **Smart Alerts** — `Blynk.logEvent()` sends a push notification automatically when AQI ≥ 100
- ✅ **Serial Debug Output** — Both Arduino and NodeMCU print human-readable logs to Serial Monitor at 115200 baud
- ✅ **Wireless Transmission** — NodeMCU handles all WiFi and cloud communication independently
- ✅ **Low-Cost & DIY-Friendly** — Built with off-the-shelf modules under $20 total; no custom PCB required

---

## 🌐 Real-World Applications

| Application | Use Case |
|:------------|:---------|
| 🏠 **Smart Homes** | Monitor indoor gas levels and humidity for a healthier living environment |
| 🏭 **Industrial Sites** | Detect hazardous gas concentrations to ensure worker safety |
| 🏥 **Hospitals & Labs** | Maintain clean-air standards for sensitive patients and equipment |
| 🌆 **Urban Monitoring** | Deploy multiple nodes to map city-wide air quality hotspots |
| 🚗 **Garages & Tunnels** | Detect CO and vehicle exhaust gases in enclosed spaces |
| 🎓 **Education & Research** | Hands-on IoT learning with real sensor data and cloud integration |

---

## 🚀 Setup & Installation Guide

### Step 1 — Install Arduino IDE

Download from [arduino.cc/en/software](https://www.arduino.cc/en/software). Version 1.8.x or 2.x is supported.

---

### Step 2 — Add ESP8266 Board Support

Go to **File → Preferences → Additional Boards Manager URLs** and add:

```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

Then go to **Tools → Board → Boards Manager**, search for `ESP8266`, and install the package by *ESP8266 Community*.

---

### Step 3 — Install Required Libraries

Go to **Sketch → Include Library → Manage Libraries** and install:

| Library | Used In | Purpose |
|:--------|:-------:|:--------|
| `AM2302-Sensor` | `sensor_data.ino` | Reads temperature & humidity from AM2302 / DHT22 |
| `ArduinoJson` | Both files | JSON serialization on Arduino; deserialization on NodeMCU |
| `SoftwareSerial` | Both files | UART serial link between Arduino and NodeMCU |
| `ESP8266WiFi` | `Wifi.ino` | WiFi connectivity on NodeMCU |
| `BlynkSimpleEsp8266` | `Wifi.ino` | Blynk cloud integration for ESP8266 |

---

### Step 4 — Configure Blynk IoT

1. Download the **Blynk IoT** app on iOS or Android
2. Sign up → create a **new template** → select `NodeMCU ESP8266`
3. Create the following **datastreams**:

| Virtual Pin | Name | Data Type |
|:-----------:|:-----|:---------:|
| `V1` | Temperature | Double |
| `V2` | Humidity | Double |
| `V3` | Air Quality Index | Integer |
| `V4` | Air Quality Status | String |

4. In your dashboard add: **Gauge** (V3), **Value Display** (V1, V2, V4), **SuperChart** (V1–V3), **Notification** widget for alerts
5. Copy your **Auth Token**, **Template ID**, and **Template Name** from the device settings

---

### Step 5 — Wire the Hardware

Connect all components as per the **Wiring & Pin Configuration** section above. Key reminders:

- **10kΩ pull-up resistor** between AM2302 DATA pin and VCC
- **Voltage divider** on Arduino D5 (TX) → NodeMCU D6 (RX) to protect the ESP8266
- All modules must share a **common GND**

---

### Step 6 — Upload `sensor_data.ino` to Arduino UNO

No code changes needed unless you change the wiring. Select:

- **Board:** Arduino UNO
- **Port:** your Arduino COM port
- Click **Upload**, then open **Serial Monitor at 115200 baud** — you should see:

```
Sent -> Temp: 34.9°C, Humidity: 35.1%, AQI: 16.0
```

---

### Step 7 — Configure & Upload `Wifi.ino` to NodeMCU

Update your credentials at the top of `Wifi.ino`:

```cpp
#define BLYNK_TEMPLATE_ID   "your_template_id"
#define BLYNK_TEMPLATE_NAME "your_template_name"
#define BLYNK_AUTH_TOKEN    "your_auth_token"

char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";
```

Then select:

- **Board:** NodeMCU 1.0 (ESP-12E Module)
- **Port:** your NodeMCU COM port
- Click **Upload**, open **Serial Monitor at 115200 baud** — you should see:

```
Received -> Temp: 34.9°C, Humidity: 35.1%, AQI: 16
```

---

### Step 8 — Launch Blynk Dashboard

Open the Blynk app and start your project. Within seconds, live readings will appear on V1–V4. If AQI reaches 100 or above, a push notification fires automatically via `Blynk.logEvent("air_quality_alert", ...)`.

---

## 📁 File Structure

```
📦 iot-air-quality-monitor/
├── 📄 sensor_data.ino    # Arduino UNO — sensor reading, AQI computation, JSON serial output
├── 📄 Wifi.ino           # NodeMCU ESP8266 — JSON parsing, Blynk upload, alert logic
└── 📄 README.md
```

---

## 🔧 Tech Stack

| Technology | Role |
|:-----------|:-----|
| **Arduino IDE (C/C++)** | Firmware development for both Arduino UNO and NodeMCU |
| **AM2302-Sensor Library** | DHT22 / AM2302 temperature & humidity acquisition |
| **ArduinoJson** | Structured JSON communication between the two microcontrollers |
| **SoftwareSerial** | UART serial link between Arduino (D5/D6) and NodeMCU (D5/D6) |
| **NodeMCU ESP8266** | WiFi connectivity and Blynk cloud upload |
| **Blynk IoT Cloud** | Real-time dashboard (V1–V4), data logging, and push alerts |
| **WiFi 802.11 b/g/n** | Wireless protocol used by ESP8266 for cloud connectivity |

---

## 🔮 Future Enhancements

| Enhancement | Description |
|:------------|:------------|
| 📡 **Multi-Platform Cloud** | Add ThingSpeak or Google Firebase for webhooks, IFTTT, and advanced data analytics |
| 📊 **ML-Based Predictions** | Use historical sensor logs to train a model predicting future AQI spikes |
| 🔋 **Battery-Powered Version** | LiPo battery + solar charging for portable, off-grid deployment |
| 🌍 **Map Visualization** | Combine with Google Maps API to display AQI across multiple deployed nodes |
| 🧪 **Additional Sensors** | Add PM2.5, PM10, CO, NO₂, and O₃ sensors for full multi-pollutant coverage |
| 📱 **Custom Mobile App** | React Native app with offline caching, local notifications, and historical charts |

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

1. **Fork** the repository
2. **Create** a feature branch: `git checkout -b feature/your-feature-name`
3. **Commit** your changes: `git commit -m "Add: description of your change"`
4. **Push** to the branch: `git push origin feature/your-feature-name`
5. **Open** a Pull Request against `main`

---

## 👨‍💻 Contributor

<div align="center">

Made with ❤️ by **Elvish**

</div>

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

<div align="center">

⭐ **Star this repo if you found it helpful!** ⭐

</div>

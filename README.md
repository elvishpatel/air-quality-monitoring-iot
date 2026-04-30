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

<br/>

| 🔵 Arduino IDE (C) | 🟢 ESP8266 WiFi | 🟠 Blynk IoT Cloud | 🟣 MQ-135 Sensor | 🔵 DHT22 Sensor |
|:-:|:-:|:-:|:-:|:-:|

<br/>

| 📊 6 AQI Levels | 📡 3 Key Parameters | 🌍 5 Applications | 📜 MIT License |
|:-:|:-:|:-:|:-:|

</div>

---

## 📌 Project Overview

This project is an **IoT-enabled smart air quality monitoring system** that tracks Air Quality Index (AQI), temperature, and humidity in real time. It leverages the `MQ-135` sensor to detect harmful gases such as CO₂, NH₃, benzene, and smoke, and the `DHT22` sensor for accurate environmental readings.

Sensor data is processed by an **Arduino UNO** and wirelessly transmitted through a **NodeMCU ESP8266** to the **Blynk IoT Cloud**, enabling remote monitoring via a smartphone dashboard — making it ideal for urban areas, industrial environments, hospitals, and smart homes.

---

## 📊 Live Dashboard Output

> **Sample readings from the Blynk IoT Dashboard:**

<div align="center">

| 🌡 Temperature | 💧 Humidity | 🌫 AQI | 🟢 Status |
|:-:|:-:|:-:|:-:|
| **34.9 °C** | **35.1 %** | **16** | **Good 😊** |

</div>

---

## 📈 AQI Index Reference

| AQI Range | Status | Health Implication | Suggested Action |
|:---------:|:------:|:-------------------|:-----------------|
| `0 – 50` | 🟢 **Good 😊** | Air quality is satisfactory | No precautions needed |
| `51 – 100` | 🟡 **Moderate 😐** | Acceptable; minor concern for sensitive groups | Sensitive groups limit outdoor activity |
| `101 – 150` | 🟠 **Sensitive Groups 😷** | Unhealthy for sensitive individuals | Seniors & children: limit outdoor time |
| `151 – 200` | 🔴 **Unhealthy 😨** | Everyone may experience health effects | Reduce outdoor exposure; wear a mask |
| `201 – 300` | 🟣 **Very Unhealthy ☠** | Serious health effects for everyone | Avoid outdoors; close windows |
| `301+` | ⚫ **Hazardous 💀** | Emergency conditions; everyone at risk | Stay indoors; seek medical attention |

---

## 🛠 Hardware Components

| Component | Role | Key Specs |
|:----------|:-----|:----------|
| **Arduino UNO** | Main microcontroller — reads sensors, computes AQI, handles serial comms | ATmega328P, 5V, 14 digital I/O |
| **NodeMCU ESP8266** | WiFi-capable MCU — receives data from Arduino and uploads to Blynk Cloud | 3.3V, 802.11 b/g/n WiFi, 11 GPIO |
| **MQ-135 Gas Sensor** | Detects CO₂, NH₃, benzene, alcohol, smoke for AQI computation | Analog + digital output, 5V |
| **DHT22 Sensor** | Measures ambient temperature and relative humidity | ±0.5°C temp accuracy, ±2–5% RH |
| **Breadboard & Jumper Wires** | Prototyping platform — no soldering required | Full-size 830 tie-points |
| **5V Power Supply** | Powers both Arduino and NodeMCU reliably | USB or dedicated 5V/2A adapter |

---

## ⚙️ System Architecture & Data Flow

```
┌─────────────┐     Analog/Digital     ┌─────────────────┐
│  MQ-135     │ ─────────────────────► │                 │
│  Gas Sensor │                        │   Arduino UNO   │
└─────────────┘                        │  (AQI Compute)  │
                                       │                 │
┌─────────────┐     Single-Wire        │                 │
│  DHT22      │ ─────────────────────► │                 │
│  Temp & RH  │                        └────────┬────────┘
└─────────────┘                                 │
                                           Serial (UART)
                                                │
                                       ┌────────▼────────┐
                                       │  NodeMCU        │
                                       │  ESP8266        │
                                       │  (WiFi Upload)  │
                                       └────────┬────────┘
                                                │
                                           WiFi / HTTP
                                                │
                                       ┌────────▼────────┐
                                       │  Blynk IoT      │
                                       │  Cloud Server   │
                                       └────────┬────────┘
                                                │
                                       ┌────────▼────────┐
                                       │  Smartphone     │
                                       │  Dashboard      │
                                       │  (Gauges/Alerts)│
                                       └─────────────────┘
```

### Step-by-Step Flow

| Step | Stage | Description |
|:----:|:------|:------------|
| **1** | 🔍 **Sense** | MQ-135 & DHT22 capture gas concentration, temperature, and humidity |
| **2** | ⚙️ **Process** | Arduino UNO reads analog/digital values and computes the AQI index |
| **3** | 📤 **Transmit** | Processed data sent to NodeMCU via UART Serial communication |
| **4** | ☁️ **Upload** | NodeMCU pushes readings to Blynk Cloud over WiFi |
| **5** | 📱 **Visualize** | Blynk app displays live gauges, charts, and sends threshold alerts |

### AQI Computation (Simplified)

```c
// MQ-135 analog output mapped to AQI index
int sensorValue = analogRead(A0);           // Raw value: 0–1023
float voltage = sensorValue * (5.0 / 1023.0); // Convert to voltage
int aqi = map(sensorValue, 0, 1023, 0, 500);  // Map to 0–500 AQI scale

// Push all readings to Blynk virtual pins
Blynk.virtualWrite(V0, aqi);         // AQI
Blynk.virtualWrite(V1, temperature); // Temperature (°C)
Blynk.virtualWrite(V2, humidity);    // Humidity (%)
```

---

## 🔌 Wiring & Pin Configuration

> ⚠️ **Important:** Use a **10kΩ pull-up resistor** on the DHT22 DATA pin. Use a **voltage divider (1kΩ + 2kΩ)** on the Arduino TX → NodeMCU RX line to step 5V down to 3.3V.

### MQ-135 Gas Sensor → Arduino UNO

| MQ-135 Pin | Arduino UNO Pin | Wire Color | Notes |
|:----------:|:---------------:|:----------:|:------|
| VCC | 5V | 🔴 Red | Power supply |
| GND | GND | ⚫ Black | Common ground |
| A0 (Analog) | A0 | 🔵 Blue | Gas concentration (0–1023) |
| D0 (Digital) | D7 | 🟢 Green | Optional threshold output |

### DHT22 Temperature & Humidity Sensor → Arduino UNO

| DHT22 Pin | Arduino UNO Pin | Wire Color | Notes |
|:---------:|:---------------:|:----------:|:------|
| VCC | 5V | 🔴 Red | Power supply (3.3–5V) |
| GND | GND | ⚫ Black | Common ground |
| DATA | D2 | 🟡 Yellow | Single-wire data — add 10kΩ pull-up to VCC |

### Arduino UNO → NodeMCU ESP8266 (Serial)

| Arduino Pin | NodeMCU Pin | Wire Color | Notes |
|:-----------:|:-----------:|:----------:|:------|
| TX (Pin 1) | RX (D7) | 🟣 Purple | **Use voltage divider — 5V → 3.3V** |
| RX (Pin 0) | TX (D8) | 🔵 Blue | Direct connection (3.3V safe) |
| GND | GND | ⚫ Black | Common ground (required) |

---

## ✨ Key Features

- ✅ **Real-Time Monitoring** — Tracks gas concentration, temperature, and humidity simultaneously with low latency
- ✅ **AQI Calculation** — Computes a simplified AQI index with color-coded health status indicators
- ✅ **Mobile Dashboard** — Interactive Blynk app with gauges, line graphs, LED indicators, and live values
- ✅ **Wireless Transmission** — ESP8266 enables WiFi-based cloud upload — no USB cable needed after setup
- ✅ **Smart Alerts** — Automatic SMS/Email notifications when AQI exceeds configurable thresholds
- ✅ **Scalable & Modular** — Easily extend with CO, PM2.5, PM10, or CO₂ sensors without redesigning the core
- ✅ **Low-Cost & DIY-Friendly** — Total BOM under $20 using off-the-shelf components; no PCB required
- ✅ **Cloud Data Logging** — Blynk stores historical sensor readings for trend analysis and export
- 🔔 **Threshold Alerts** — Push notifications via Blynk when AQI, temperature, or humidity cross set limits

---

## 🌐 Real-World Applications

| Application | Use Case |
|:------------|:---------|
| 🏠 **Smart Homes** | Track indoor pollution, CO₂ buildup, and humidity for a healthier living environment |
| 🏭 **Industrial Sites** | Ensure worker safety in factories, warehouses, and chemically hazardous environments |
| 🏥 **Hospitals & Labs** | Maintain clean-air standards for patients, surgical rooms, and sensitive equipment |
| 🌆 **Urban Monitoring** | Deploy across city grids to map pollution hotspots and support policy decisions |
| 🚗 **Vehicle Emissions** | Detect harmful gases in parking garages, tunnels, or emission testing stations |
| 🎓 **Education & Research** | A perfect hands-on IoT learning project for students and makers |

---

## 🚀 Setup & Installation Guide

### Step 1 — Install Arduino IDE

Download and install the latest Arduino IDE from [arduino.cc/en/software](https://www.arduino.cc/en/software). Version 1.8.x or 2.x is supported.

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

| Library | Purpose |
|:--------|:--------|
| `Blynk` | IoT Cloud communication with the Blynk platform |
| `DHT sensor library` | Reading temperature and humidity from DHT22 |
| `Adafruit Unified Sensor` | Sensor abstraction layer (required by DHT library) |

---

### Step 4 — Configure Blynk IoT

1. Download the **Blynk IoT** app on iOS or Android
2. Sign up and create a **new device** → select `NodeMCU ESP8266`
3. Copy the **Auth Token** from device settings
4. Add widgets to the dashboard:

| Widget | Virtual Pin | Range / Type |
|:-------|:-----------:|:-------------|
| Gauge | `V0` | AQI — range 0 to 500 |
| Value Display | `V1` | Temperature in °C |
| Value Display | `V2` | Humidity in % |
| SuperChart | `V0, V1, V2` | Historical trend graph |
| Notification | — | AQI threshold alerts |

---

### Step 5 — Wire the Hardware

Connect all components as described in the **Wiring & Pin Configuration** section above. Key reminders:

- Add a **10kΩ pull-up resistor** between DHT22 DATA and VCC
- Use a **voltage divider** (1kΩ + 2kΩ) on Arduino TX → NodeMCU RX to prevent damage
- Ensure all components share a **common GND**

---

### Step 6 — Configure & Upload the Sketch

Open the Arduino sketch and update these lines:

```c
#define BLYNK_AUTH_TOKEN  "your_auth_token_here"

char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";
```

Then:

1. Select **Tools → Board → NodeMCU 1.0 (ESP-12E Module)**
2. Select the correct **COM Port** under Tools → Port
3. Click **Upload** and open **Serial Monitor** at `9600 baud` to verify

---

### Step 7 — Launch Blynk Dashboard

Open the Blynk app, start your project, and watch the readings update in real time. You should see live AQI, temperature, and humidity values flowing within seconds of upload.

---

## 🔧 Tech Stack

| Technology | Role |
|:-----------|:-----|
| **Arduino IDE (C/C++)** | Firmware development for sensor reading and AQI computation |
| **NodeMCU ESP8266** | WiFi-enabled data transmission to cloud |
| **Blynk IoT Cloud** | Real-time dashboard, data logging, and push notifications |
| **UART Serial** | Communication protocol between Arduino and NodeMCU |
| **MQ-135 + DHT22** | Primary sensing hardware for gas and environmental data |
| **WiFi 802.11 b/g/n** | Wireless protocol for cloud connectivity |

---

## 🔮 Future Enhancements

| Enhancement | Description |
|:------------|:------------|
| 📡 **Multi-Platform Cloud** | Integrate ThingSpeak or Google Firebase for webhooks, IFTTT, and advanced analytics |
| 📊 **ML Predictions** | Export sensor logs for time-series ML models to predict future AQI levels |
| 🔋 **Battery-Powered Version** | Add LiPo battery + solar charging for fully portable, off-grid deployment |
| 🌍 **Map Integration** | Use Google Maps API to visualize AQI readings geographically across multiple nodes |
| 🧪 **Additional Sensors** | Add PM2.5, PM10, CO, NO₂, and O₃ for comprehensive multi-pollutant monitoring |
| 📱 **Custom Mobile App** | Build a React Native app with custom UX, local notifications, and offline support |

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

1. **Fork** the repository
2. **Create** a feature branch: `git checkout -b feature/your-feature-name`
3. **Commit** your changes: `git commit -m "Add: your feature description"`
4. **Push** to the branch: `git push origin feature/your-feature-name`
5. **Open** a Pull Request against `main`

Please make sure your code is well-commented and follows the existing style.

---

## 👨‍💻 Contributor

<div align="center">

Made with ❤️ by **Elvish**

*IoT Air Quality Monitoring System — Open Source & Community Driven*

</div>

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

```
MIT License — Free to use, modify, and distribute with attribution.
```

---

<div align="center">

⭐ **Star this repo if you found it helpful!** ⭐

</div>

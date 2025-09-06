# 🌍 IoT-Based Air Quality Monitoring System

## 📌 Overview
This project is an IoT-enabled smart air quality monitoring system that tracks **Air Quality Index (AQI)**, **temperature**, and **humidity** in real time. It leverages the **MQ-135 sensor** to detect harmful gases (e.g., CO₂, NH₃, benzene, smoke) and the **DHT22 sensor** for environmental readings.

The system provides actionable insights about pollution levels by sending data to the **Blynk IoT Cloud Platform**, enabling remote access via a smartphone. It is especially useful for urban areas, industries, and indoor environments where air quality impacts health directly.

## 🛠 Hardware Used
- **Arduino UNO** – Sensor data acquisition  
- **NodeMCU ESP8266** – WiFi-enabled data transmission  
- **MQ-135 Gas Sensor** – Detects harmful gases & air quality levels  
- **DHT22 Sensor** – Measures temperature & humidity  
- Jumper Wires & Breadboard  
- Power Supply  

## ⚙️ How It Works
1. **Data Acquisition** – MQ-135 detects gas concentration, and DHT22 measures temperature & humidity.  
2. **Processing** – Arduino UNO processes the raw data and formats it.  
3. **Communication** – Data is transmitted to NodeMCU ESP8266 via serial.  
4. **Cloud Upload** – ESP8266 pushes the readings to the Blynk IoT Cloud over WiFi.  
5. **Visualization** – The Blynk app dashboard displays gauges, graphs, and indicators in real time.  
6. **AQI Calculation** – The system computes a simplified AQI index based on the collected data.  

## 📊 Features
- ✅ Real-time air quality monitoring (gas + temperature + humidity)  
- ✅ AQI calculation with user-friendly health indicators  
- ✅ Mobile app dashboard with interactive charts, gauges, and status alerts  
- ✅ Wireless IoT data transmission via ESP8266  
- ✅ Scalable design – can integrate more sensors (CO, PM2.5, PM10, CO₂, etc.)  
- ✅ Low-cost, portable, and DIY-friendly system  
- 🔔 Automatic alerts via SMS/Email when AQI crosses a threshold  

## 🌐 Real-Life Applications
- 🏠 **Smart Homes** – Track indoor pollution and humidity levels.  
- 🏭 **Industrial Monitoring** – Ensure worker safety in polluted areas.  
- 🏥 **Hospitals & Labs** – Monitor clean-air conditions for sensitive patients/equipment.  
- 🌆 **Urban Cities** – Collect large-scale data for pollution analysis.  
- 🚗 **Vehicle Emissions** – Detect harmful gases in garages or tunnels.  

## 🚀 Output Example
**Live Dashboard Readings (Blynk IoT):**
- 🌡 Temperature: 34.9°C  
- 💧 Humidity: 35.1%  
- 🌫 AQI: 16 (Good 😊)  

**AQI Status Levels:**
| AQI | Status |
|-----|-------|
| 0–50 | Good 😊 |
| 51–100 | Moderate 😐 |
| 101–150 | Unhealthy for Sensitive Groups 😷 |
| 151–200 | Unhealthy 😨 |
| 201–300 | Very Unhealthy ☠ |
| 301+ | Hazardous 💀 |

## 🔧 Tech Stack
- Arduino IDE (C)  
- NodeMCU ESP8266 WiFi Module  
- Blynk IoT Cloud Platform  
- Serial Communication Protocols  

## 📸 Project Preview
**Hardware Setup**  
![Hardware Setup](https://github.com/user-attachments/assets/5e4d882c-b7b4-436a-b9a9-3b3d738b1e14)  

**Mobile Dashboard**  
![Mobile Dashboard](https://github.com/user-attachments/assets/4f8ff33d-82e1-42b9-a7d9-a06d8d811a17)  

## ⚡ Setup & Blynk IoT Connection
Follow these steps to set up and connect the project to Blynk IoT:

1. **Install Arduino IDE**
   - Download and install from [Arduino Official Website](https://www.arduino.cc/en/software).

2. **Add ESP8266 Board to Arduino IDE**
   - Go to **File → Preferences → Additional Boards Manager URLs**
   - Add: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - Open **Tools → Board → Boards Manager**, search for **ESP8266**, and install.

3. **Install Required Libraries**
   - In Arduino IDE, go to **Sketch → Include Library → Manage Libraries**  
     Install:
     - `Blynk`
     - `DHT sensor library`
     - `Adafruit Unified Sensor`

4. **Configure Blynk**
   - Download **Blynk IoT** app on your smartphone.  
   - Sign up and create a **new device** (choose NodeMCU ESP8266).  
   - Copy the **Auth Token** generated for your device.

5. **Connect Sensors**
   - Connect MQ-135 and DHT22 sensors to the Arduino as per the wiring diagram.  
   - Connect Arduino to NodeMCU ESP8266 for serial communication.

6. **Upload Arduino Code**
   - Open the Arduino sketch provided in the repository.  
   - Replace the placeholder **BLYNK_AUTH_TOKEN** with your actual token.  
   - Set your **WiFi SSID and Password** in the code.  
   - Select the correct **Board** and **Port**, then upload the sketch.

7. **Launch Blynk App Dashboard**
   - Open the app and start your project.  
   - Add **gauges, value displays, and notifications** to visualize AQI, temperature, and humidity.  
   - The dashboard will update in **real time** as data is transmitted via ESP8266.

## 🔮 Future Enhancements
- 📡 Integration with ThingSpeak or Google Firebase for advanced analytics  
- 📊 Exporting sensor logs for trend analysis & ML-based predictions  
- 🔋 Battery-powered portable version  
- 🌍 Integration with Google Maps API to visualize AQI levels across locations  

## 🤝 Contributing
Contributions, issues, and feature requests are welcome!  
Feel free to fork this repo and submit pull requests.  

## 👨‍💻 Contributor
Made with ❤ by **Elvish**  

## 📜 License
This project is licensed under the **MIT License** – see the [LICENSE](LICENSE) file for details.

# 🌱 Smart Irrigation System Using IoT

This project presents an **automated irrigation system** based on **Arduino UNO**, **ESP32**, and the **Blynk IoT** platform. The system aims to **optimize water usage** by monitoring environmental conditions in real time and controlling a water pump based on user-defined modes (manual or automatic).

---

## 🚀 Features

- 📡 Real-time monitoring of:
  - Temperature and humidity (DHT22 sensor)
  - Soil moisture level
  - Rain detection
  - Water flow rate
  - Water level in the tank
- 📲 Integration with **Blynk IoT App** for remote control and data visualization
- 🔁 Two operation modes:
  - **Manual Mode**: User controls the pump directly via Blynk
  - **Automatic Mode**: Pump is activated when:
    - Soil moisture < 40%
    - AND temperature < 30°C

---

## 🧠 How It Works

### 1. Arduino UNO (Sensor Node)

- Reads sensor data (DHT22, soil moisture, rain, water flow, and water level).
- Formats the data into a structured string like:
humidity:52.3;temp:26.1;moisture:38;flow:1.5;level:70;rain:no rain;
- Sends the string to the ESP32 via **UART Serial Communication**.

### 2. ESP32 (IoT Node)

- Receives and parses the sensor data string using the `readFromArduino()` function.
- Sends each value to **Blynk** using appropriate **virtual pins**:
- `V2` → Soil moisture
- `V3` → Flow rate
- `V4` → Humidity
- `V5` → Temperature
- `V6` → Rain status
- `V7` → Water level
- Listens for commands from Blynk to control the pump (`pump_on`, `pump_off`, `auto_mode`).
- Forwards commands to Arduino for execution.

### 3. Communication Protocol

- **Arduino ➜ ESP32**: Sensor data string
- **ESP32 ➜ Arduino**: Plain-text pump control commands

---

## 💻 Technologies Used

- **C++ / Arduino**
- **ESP32 and Arduino UNO**
- **Blynk IoT Platform**
- **Serial Communication (UART)**
- **Sensors**:
- DHT22 (Temperature & Humidity)
- Soil Moisture Sensor
- Rain Sensor
- Water Flow Sensor
- Ultrasonic Sensor (Water Level)

---

## 📁 File Structure

/project-root
├── arduino/ # Code for Arduino UNO (sensors, pump control)
├── esp32/ # Code for ESP32 (Blynk, serial communication)
└── README.md # Project documentation


## 📌 Conclusion

This project shows how IoT technology can be applied to create **smart irrigation systems**, enabling water-saving agriculture through **remote monitoring** and **automated control**.

---

<div align="center">

# 🌾 SMART FIELD AI

### NEXT-GENERATION AI IRRIGATION & FERTILIZER AUTOMATION SYSTEM

<img src="images/dashboard-ui.png" width="100%"/>

<br>

<img src="https://img.shields.io/badge/AI-SMART%20AGRICULTURE-00C853?style=for-the-badge">
<img src="https://img.shields.io/badge/ESP8266-IOT%20SYSTEM-2979FF?style=for-the-badge">
<img src="https://img.shields.io/badge/STATUS-LIVE%20MONITORING-FF6D00?style=for-the-badge">
<img src="https://img.shields.io/badge/REALTIME-WEB%20DASHBOARD-9C27B0?style=for-the-badge">

<br><br>

> Intelligent Embedded Farming System with Real-Time Monitoring, Smart Irrigation, Fertilizer Automation, and Live IoT Dashboard

</div>

---

# 🚀 SYSTEM OVERVIEW

Smart Field AI is an advanced embedded agriculture automation platform designed to optimize irrigation and fertilizer distribution using real-time sensor data and intelligent control logic.

The system continuously monitors:

- 🌱 Soil Moisture
- 💧 Tank Water Level
- ⚡ Pump Runtime
- 🧪 Fertilizer Mixing Ratio

and automatically controls pumps using an ESP8266-powered embedded IoT architecture.

---

# 🧠 HOW THE SYSTEM WORKS

```text
                ┌────────────────────┐
                │ Soil Moisture Data │
                └─────────┬──────────┘
                          │
                          ▼
                ┌────────────────────┐
                │ ESP8266 Controller │
                └─────────┬──────────┘
                          │
         ┌────────────────┼────────────────┐
         │                │                │
         ▼                ▼                ▼

 ┌─────────────┐  ┌──────────────┐  ┌──────────────┐
 │ Water Pump  │  │ Fertilizer   │  │ Tank Level   │
 │ Automation  │  │ Mixing Pump  │  │ Monitoring   │
 └─────────────┘  └──────────────┘  └──────────────┘
         │                │                │
         └────────────────┼────────────────┘
                          │
                          ▼

              ┌─────────────────────┐
              │ Real-Time Dashboard │
              └─────────────────────┘
```

---

# 🌐 LIVE DASHBOARD INTERFACE

<div align="center">

<img src="images/dashboard-ui.png" width="95%"/>

</div>

---

# 📡 REAL-TIME SENSOR VISUALIZATION

<table>
<tr>
<td align="center">

## 🌱 Soil Moisture

The soil moisture sensor continuously checks soil condition.

- Low Moisture → Water Pump ON
- Optimal Moisture → Pump OFF

</td>

<td align="center">

## 💧 Water Tank Monitoring

HC-SR04 ultrasonic sensor measures tank level in real time.

- Tank Empty → Dashboard Alert
- Tank Full → Auto Stop Protection

</td>
</tr>
</table>

---

# ⚡ PUMP CONTROL SYSTEM

```text
Pump 1 → Water Irrigation System
Pump 2 → Fertilizer Mixing System
```

The ESP8266 controls both pumps using the L298N motor driver.

### 🌊 Water Pump
Automatically irrigates plants when soil moisture drops below threshold.

### 🧪 Fertilizer Pump
Injects fertilizer solution based on selected concentration level from dashboard slider.

---

# 📊 LIVE SYSTEM STATUS

<div align="center">

| Parameter | Live Monitoring |
|---|---|
| Soil Moisture | ✅ Real-Time |
| Tank Level | ✅ Real-Time |
| Pump Runtime | ✅ Real-Time |
| Fertilizer Mix | ✅ Adjustable |
| AUTO Mode | ✅ Enabled |
| Dashboard Updates | ✅ Wireless |

</div>

---

# 🌱 REAL WORKING PROTOTYPE

<div align="center">

<img src="images/prototype.jpeg" width="85%"/>

</div>

---

# 🧪 AI SMART FERTILIZER MIXER

<div align="center">

<img src="images/smart-fertilizer-system.jpeg" width="85%"/>

</div>

---

# 🔌 HARDWARE CONNECTION ARCHITECTURE

<div align="center">

<img src="images/accurate-connections.png" width="95%"/>

</div>

---

# ⚡ HARDWARE COMPONENTS

| Component | Function |
|---|---|
| ESP8266 NodeMCU | Main IoT Controller |
| L298N Motor Driver | Pump Driver Control |
| HC-SR04 Sensor | Water Level Detection |
| Soil Moisture Sensor | Soil Analysis |
| Water Pump | Irrigation |
| Fertilizer Pump | Nutrient Mixing |
| 18650 Battery Pack | Portable Power System |

---

# 📈 SYSTEM DATA FLOW

```text
Sensors → ESP8266 → AI Logic Engine
                ↓
         Pump Automation
                ↓
      Live Web Dashboard
                ↓
      Real-Time User Control
```

---

# 🔌 PIN CONNECTIONS

## L298N → ESP8266

| L298N | NodeMCU |
|---|---|
| IN1 | D1 |
| IN2 | D2 |
| IN3 | D5 |
| IN4 | D6 |
| 5V | VIN |
| GND | GND |

---

## HC-SR04 Ultrasonic Sensor

| HC-SR04 | NodeMCU |
|---|---|
| VCC | 3V3 |
| GND | GND |
| TRIG | D7 |
| ECHO | D8 |

---

## Soil Moisture Sensor

| Sensor | NodeMCU |
|---|---|
| VCC | 3V3 |
| GND | GND |
| AO | A0 |

---

# 🌐 WEB DASHBOARD

The ESP8266 creates a dedicated WiFi network for live monitoring and wireless control.

## WiFi Credentials

```text
SSID: SmartField
Password: 12345678
```

---

# 📡 DASHBOARD ACCESS

```text
http://192.168.4.1
```

---

# 🛠️ TECHNOLOGY STACK

| Technology | Purpose |
|---|---|
| ESP8266 | Embedded IoT Controller |
| Arduino C++ | Firmware Development |
| HTML/CSS/JavaScript | Dashboard Interface |
| L298N | Motor Driver |
| Sensors | Environmental Monitoring |
| WiFi Server | Real-Time Communication |

---

# 📂 PROJECT STRUCTURE

```text
Smart-Field-AI/
│
├── images/
│   ├── dashboard-ui.png
│   ├── prototype.jpeg
│   ├── smart-fertilizer-system.jpeg
│   ├── wiring-diagram.png
│   └── accurate-connections.png
│
├── esp_8266.ino
├── README.md
├── LICENSE
└── .gitignore
```

---

# 🚀 SETUP GUIDE

## 1️⃣ Install Arduino IDE

Install:
- ESP8266 Board Package

---

## 2️⃣ Open Firmware

```text
esp_8266.ino
```

---

## 3️⃣ Select Board

```text
NodeMCU 1.0 (ESP8266)
```

---

## 4️⃣ Upload Firmware

Connect ESP8266 and upload code.

---

# 🌟 FUTURE IMPROVEMENTS

- ☁️ Cloud Analytics
- 📱 Mobile Application
- 🌦️ Weather Prediction
- 🤖 AI Crop Recommendation
- 🔔 Telegram Notifications
- 🌍 Remote Monitoring
- 📊 Historical Graph Analytics

---

# 📚 LEARNING OUTCOMES

This project demonstrates:

- Embedded Systems
- IoT Automation
- Smart Agriculture
- Sensor Integration
- Real-Time Dashboards
- Automation Logic
- Wireless Communication
- AI-Based Monitoring

---

# 👨‍💻 AUTHOR

## Adunuri Surender

AI • Robotics • IoT • Embedded Systems Developer

---

# 📄 LICENSE

MIT License

---

# ⭐ SUPPORT

If you like this project:

⭐ Star the repository  
🍴 Fork the project  
📢 Share with others  

---

<div align="center">

# 🌱 AI • AUTOMATION • SMART FARMING • EMBEDDED SYSTEMS

</div>

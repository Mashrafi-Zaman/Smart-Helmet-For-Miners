<h1 align="center">SMART HELMET FOR MINERS</h1>


<p align="center">
  <img src="images/h.png" alt="System Overview" width="500"/>
</p>

---

## 🧠 Motivation

Miners and cave explorers often face hazardous working environments, including poor air quality, risk of falls, and the need for emergency communication. 

This smart helmet aims to:
- Monitor environmental conditions in real-time.
- Detect accidents like falls.
- Provide an SOS alert feature for emergencies.

By integrating sensors, wireless communication, and microcontroller technologies, the helmet improves safety and enhances emergency response—fulfilling the increasing need for smart safety gear in industrial settings.

---

## ⚙️ System Implementation

<p align="center">
  <img src="images/r.png" alt="System Implementation" width="500"/>
</p>

The helmet comprises the following modules:

- **Gas Detection:** MQ-2 sensor for identifying hazardous gases.
- **Fall Detection:** MPU6050 accelerometer and gyroscope.
- **Environmental Monitoring:** DHT11 for temperature and humidity.
- **Communication Modules:**  
  - ESP8266 for Wi-Fi-based data transmission  
  - LoRa for long-range emergency messaging
- **Emergency Signaling:** SOS button and buzzer

---

## 🔩 Component Breakdown

| Component       | Function                                                                 |
|----------------|--------------------------------------------------------------------------|
| **MQ-4 Sensor** | Detects methane and combustible gases. Outputs analog signal.           |
| **MPU6050**     | 3-axis gyroscope and accelerometer for motion and fall detection.       |
| **DHT11**       | Measures temperature and humidity (digital output).                     |
| **ESP8266**     | Wi-Fi communication for real-time data visualization.                   |
| **LoRa**        | Low-power, long-range communication in remote areas.                    |
| **Buzzer**      | Audible alerts for hazards or emergencies.                              |
| **Power Supply**| Rechargeable Li-ion battery for portable energy.                        |

---

## 🔁 System Flowchart

<p align="center">
  <img src="images/f.png" alt="System Flowchart" width="500"/>
</p>

### 🧱 Flowchart Block Explanation:

1. **Start** – Begin system operation  
2. **Initialize Wi-Fi and Sensor Modules** – Setup phase for Wi-Fi & sensors  
3. **Read Sensor Values** – Collect data (temperature, humidity, gas, motion)  
4. **Update Webpage with Sensor Values** – Live environmental updates  
5. **Hazard Detected?** – Decision based on sensor thresholds  
   - ✅ Yes → Trigger buzzer and send data via LoRa  
   - ❌ No → Continue monitoring  
6. **SOS Button Pressed?** – Manual override for emergency alert  
7. **LoRa Receiver Shows Data** – Sensor data is displayed remotely  
8. **End** – The loop restarts for continuous monitoring

---

## 🔌 ESP8266 ↔ LoRa Wiring Diagram

<p align="center">
  <img src="images/c.png" alt="ESP8266 to LoRa Connection" width="450"/>
</p>

---

## ✅ Results

- 🔥 Gas detection accuracy: **~90%** (post calibration)
- 🧍‍♂️ Fall detection via MPU6050: **Jerk threshold = 300,000 m/s³**
- 🌐 Real-time environment dashboard accessible via local IP
- 📡 LoRa alert transmission time: **< 1 second**
- 🎯 Lightweight, ergonomic design suitable for practical use

---

## 📊 Wi-Fi Dashboard

<p align="center">
  <img src="images/d.png" alt="Wi-Fi Dashboard" width="400"/>
</p>

Access via: **[192.168.4.1](http://192.168.4.1)**

---

## 📡 LoRa Receiver Output

<p align="center">
  <img src="images/l.png" alt="LoRa Output" width="600"/>
</p>

---


 ┗ 📄 main_code.ino

 **SMART** **HELMET** **FOR** **MINERS**
---


<p align="center"> <img src="images/h.png" alt="System Overview" width="500"/> </p>


---
**Motivation:**

 Miners and cave explorers often face hazardous working environments,
 including poor air quality, the risk of falls, and the need for
 emergency communication. To mitigate these risks, a smart helmet can
 provide real-time monitoring of environmental conditions, detect
 potential accidents like falls, and offer an SOS feature for
 emergencies. By leveraging sensors, wireless communication, and
 microcontroller technology, this project aims to enhance safety and
 improve response times in critical situations. The development of such
 a helmet aligns with the growing need for smart safety equipment in
 industries, ensuring both worker safety and operational efficiency.

---
 **Implementation:**

<p align="center"> <img src="images/r.png" alt="System Overview" width="500"/> </p>


The system consists of:

 • **Gas** **detection**: MQ-2 sensor to measure hazardous gas levels.

 • **Fall** **detection**: MPU6050 accelerometer and gyroscope to
 monitor abrupt movements.

 • **Environmental** **monitoring**: DHT11 sensor for temperature and
 humidity.

 • **Communication** **modules**: ESP8266 for Wi-Fi-based data
 transmission and LoRa for remote messaging.

 • **Emergency** **signalling**: SOS button and buzzer for immediate
 alerts.

---
**Component** **Basics:**

 1\. **MQ-4** **Sensor**: Detects methane and other combustible gases,
 providing analog output proportional to gas concentration.
 
 2\. **MPU6050**: Combines a 3-axis gyroscope and accelerometer for
 precise motion tracking.
 
 3\. **DHT11**: Measures temperature and humidity with digital output.
 
 4\. **ESP8266**: Enables Wi-Fi communication for real-time data
  visualization. 
  
 5\. **LoRa**: Ensures long-range, low-power> communication in remote areas. 
 
 6\.**Buzzer**: Provides audible alerts during emergencies.
 
 7\. **Power** **Supply**: A rechargeable Li-ion battery powers the
 system efficiently.

---

**Flowchart:**

<p align="center"> <img src="images/f.png" alt="System Flowchart" width="500"/> </p>

let's break down each block in the flowchart:

**1.** **Start**:

 This is the initial point of the system's operation.

**2.** **Initialize** **Wi-Fi** **and** **Sensor** **Modules:**

This block represents the setup phase where the system establishes
communication with the Wi-Fi network and activates the sensors
responsible for monitoring the environment.

**3.** **Read** **Sensor** **Values:**

 This block signifies the continuous process of acquiring data from the
 sensors. These values could represent various environmental parameters like
 temperature, humidity, gas levels, etc.

**4.** **Update** **Webpage** **with** **Sensor** **Values:**

The system takes the sensor data and dynamically updates a webpage with
the current readings. This allows users to remotely monitor the
environment in real-time.

**5.** **Hazard** **Detected?** **(Decision** **Block):**

This is a crucial decision point. The system analyzes the sensor data to
determine if any hazardous condition exists. This could involve
comparing the readings against predefined thresholds or applying more
complex algorithms.

**6.** **Continue** **Monitoring:**

If no hazard is detected, the system proceeds to the next monitoring
cycle, starting from "Read Sensor Values."

**7.** **Trigger** **Buzzer** **and** **Send** **Data** **via**
**LoRa:**

If a hazard is detected, the system activates an alarm (buzzer) to alert
nearby individuals. Additionally, it transmits the sensor data using the
LoRa wireless communication protocol to a remote receiver.

**8.** **LoRa** **Receiver** **Shows** **Data:**

The LoRa receiver displays the received sensor data, providing valuable
information to those monitoring the environment from a distance.

**9.** **SOS** **Button** **Triggered?** **(Decision** **Block):**

This block introduces a manual trigger mechanism. If an SOS button is
pressed, the system bypasses the hazard detection logic and directly
proceeds to "Trigger Buzzer and Send Data via LoRa." This allows for
immediate emergency alerts.

**10.** **End:**

The system continues to operate in this loop, monitoring the environment
and responding to hazards or manual triggers.

---
**Wire** **connection** **between** **ESP8266** **and** **LoRa**

<p align="center"> <img src="images/c.png" alt="ESP8266 to LoRa Connection" width="450"/> </p>
 
 ---
 **Result:**

 The Smart Helmet successfully:

 • Detects hazardous gases with an accuracy of 90% based on calibration
 tests. 
 
 • Identifies falls using a jerk magnitude threshold of 300,000
 m/s³.
 
 • Provides real-time environmental data on a web-based dashboard.
 
 • Sends emergency alerts through LoRa in under 1 second.
 
 • Ensures lightweight and ergonomic design for practical use.

 ---
**Dashboard** **(192.168.4.1)** 


<p align="center"> <img src="images/d.png" alt="Wi-Fi Dashboard" width="400"/> </p>

---
**LoRa** **Receiver** 


<p align="center"> <img src="images/l.png" alt="LoRa Output" width="600"/> </p>






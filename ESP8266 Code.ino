#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <MPU6050.h>
#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 16
#define dio0 5

// DHT sensor setup
#define DHTPIN 4       // DHT11 data pin connected to GPIO10 (S3 on NodeMCU)
#define DHTTYPE DHT11  // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

const int gasSensorPin = A0;  // Analog pin connected to the MQ-4 sensor
const int buzzerPin = 10;     // GPIO10
// Calibration constants
const float airValue = 0.7;  // Voltage in clean air (calibrate as needed)
const float gasThreshold = 60;

String json;

const float fallThreshold = 300000;  // Adjust this value to suit your needs (in m/s^3)
const int sampleInterval = 10;      // Interval in milliseconds between readings
MPU6050 mpu;

// Define pins for I2C
#define SDA_PIN 2  // GPIO2 (D4 on NodeMCU)
#define SCL_PIN 0  // GPIO0 (D3 on NodeMCU)

// Variables to store previous acceleration values
float prevAccX = 0.0, prevAccY = 0.0, prevAccZ = 0.0;
bool fall_detected = false;
bool gas_detected = false;
float gasConcentration = 0.0;

unsigned long previousMillis = 0;          // Stores the last time the message was sent
const unsigned long delayInterval = 3000;  // Set your delay interval in milliseconds


unsigned long fallStartTime = 0;          // Timestamp of when the fall was first detected
const unsigned long fallDuration = 5000;  // 5 seconds in milliseconds

// Wi-Fi Access Point credentials
const char *ssid = "Smart Helmet 01";  // AP Wi-Fi name
const char *password = "1234567890";   // AP Wi-Fi password

WiFiServer server(80);

void initializeMPU() {
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1)
      ;
  }
  Serial.println("MPU6050 initialized");
}

void initLoRa() {
  LoRa.setPins(ss, rst, dio0);

  // Retry mechanism to initialize LoRa
  while (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed! Retrying...");
    delay(1000);
  }

  Serial.println("LoRa initialization successful!");
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  Wire.begin(SDA_PIN, SCL_PIN);  // Initialize I2C with SDA and SCL pins
  initializeMPU();               // Initialize the MPU6050

  pinMode(gasSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Start ESP8266 as Access Point
  WiFi.softAP(ssid, password);

  // Start the server
  server.begin();

  // Print ESP8266 AP IP address
  Serial.print("ESP8266 Access Point IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Attempt to initialize LoRa module
  initLoRa();
}

void loop() {

  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();
    static unsigned long lastTime = 0;
    if (millis() - lastTime >= sampleInterval) {
      lastTime = millis();

      // Verify sensor connection
      if (!mpu.testConnection()) {
        initializeMPU();  // Attempt to reinitialize the sensor
        return;           // Skip this iteration
      }

      // Read accelerometer data
      int16_t ax, ay, az;
      mpu.getAcceleration(&ax, &ay, &az);

      // Convert accelerometer readings from m/s^2 to g (1 g = 9.8 m/s^2)
      float acceleration_mg_x = ax / 9.8;
      float acceleration_mg_y = ay / 9.8;
      float acceleration_mg_z = az / 9.8;

      // Calculate jerk by taking the derivative of acceleration with respect to time
      float jerkX = (acceleration_mg_x - prevAccX) / (sampleInterval / 1000.0);
      float jerkY = (acceleration_mg_y - prevAccY) / (sampleInterval / 1000.0);
      float jerkZ = (acceleration_mg_z - prevAccZ) / (sampleInterval / 1000.0);

      // Update previous acceleration values for the next iteration
      prevAccX = acceleration_mg_x;
      prevAccY = acceleration_mg_y;
      prevAccZ = acceleration_mg_z;

      // Calculate the magnitude of jerk
      float jerkMagnitude = sqrt(jerkX * jerkX + jerkY * jerkY + jerkZ * jerkZ);

      if (jerkMagnitude > fallThreshold) {
        fall_detected = true;
        fallStartTime = millis();  // Record the time when the fall is detected
      }
    }

    // Maintain fall_detected as true for 5 seconds
    if (fall_detected && millis() - fallStartTime > fallDuration) {
      fall_detected = false;  // Reset after 5 seconds
    }

    // Read gas sensor data
    int sensorValue = analogRead(gasSensorPin);
    float sensorVoltage = sensorValue * (3.3 / 1023.0);
    Serial.println(sensorVoltage);
    gasConcentration = (sensorVoltage - airValue) * 100;  // Example formula
         if (gasConcentration < 0) {
             gasConcentration = 0;  // Ensure gasConcentration is non-negative
          }
    gas_detected = gasConcentration > gasThreshold;

    
      

    if (request.indexOf("/sos") != -1) {
     digitalWrite(buzzerPin, HIGH);  // Activate buzzer
     Serial.println("SOS Button Pressed - Buzzer ON");
     client.println("HTTP/1.1 200 OK");
     client.println("Content-Type: text/plain; charset=utf-8");
     client.println("Connection: close");
     client.println();
     client.println("SOS activated");
     return;
    }  


    // Serve the main page
    if (request.indexOf("/getData") != -1) {
      // Respond with JSON data for AJAX requests
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      // Check if readings are valid
      if (isnan(temperature) || isnan(humidity)) {
        temperature = -1;  // Indicate an error
        humidity = -1;
      }

      if (gas_detected || temperature > 40.0 || fall_detected) {
        digitalWrite(buzzerPin, HIGH);
      } else {
        digitalWrite(buzzerPin, LOW);
      }

      // Send JSON response
      String json2 = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"fall_detected\":\"" + (fall_detected ? "Yes" : "No") + "\",\"gas_detected\":\"" + (gas_detected ? "Yes" : "No") + "\",\"gasConcentration\":" + String(gasConcentration) + "}";

      json = json2;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json; charset=utf-8");
      client.println("Connection: close");
      client.println();
      client.println(json2);

      return;
    } else {
      // Serve the HTML page
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html; charset=utf-8");  // UTF-8 encoding
      client.println("Connection: close");
      client.println();
      client.println(R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
          <title>Smart Helmet Dashboard</title>
          <style>
            body {
              font-family: Arial, sans-serif;
              margin: 0;
              padding: 0;
              background-color: #f0f0f0;
              color: #333;
            }
            .container {
              max-width: 800px;
              margin: 0 auto;
              padding: 20px;
              text-align: center;
            }
            .card {
              background: #fff;
              border-radius: 8px;
              box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
              margin: 20px 0;
              padding: 20px;
              text-align: center;
              transition: 0.3s;
            }
            .card:hover {
              box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);
            }
            h1 {
              color: #0078d7;
            }
            .status {
              font-size: 1.5em;
              margin: 10px 0;
            }
            .status span {
              font-weight: bold;
            }
            .gas-danger {
              color: red;
            }
            .gas-safe {
              color: green;
            }
            .fall-danger {
              color: red;
            }
            .fall-safe {
              color: green;
            }
            button {
             background-color: #0078d7; /* Button background color */
             color: white;             /* Button text color */
             border: none;             /* Remove border */
             border-radius: 4px;       /* Rounded corners */
             padding: 10px 20px;       /* Padding around the text */
             font-size: 16px;          /* Font size */
             cursor: pointer;          /* Change cursor to pointer on hover */
             transition: background-color 0.3s ease; /* Smooth hover transition */
            }

            button:hover {
             background-color: #005bb5; /* Darker shade for hover effect */
            }

            button:active {
             background-color: #003e80; /* Even darker shade when clicked */
             transform: scale(0.95);    /* Slightly shrink the button on click */
            }
            
          </style>
          <script>
            function updateData() {
              fetch('/getData')
                .then(response => response.json())
                .then(data => {
                  document.getElementById('temperature').innerText = data.temperature + " °C";
                  document.getElementById('humidity').innerText = data.humidity + " %";
                  document.getElementById('fallStatus').innerText = data.fall_detected;
                  document.getElementById('fallStatus').className = data.fall_detected === "Yes" ? "fall-danger" : "fall-safe";
                  document.getElementById('gasStatus').innerText = data.gas_detected === "Yes" ? "Yes" : "No";
                  document.getElementById('gasConcentration').innerText = data.gasConcentration + " ppm";
                  document.getElementById('gasStatus').className = data.gas_detected === "Yes" ? "gas-danger" : "gas-safe";
                })
                .catch(error => console.error('Error:', error));
            }
            setInterval(updateData, 100); // Update every 100 milisecond
          </script>
        </head>
        <body onload="updateData()">
          <div class="container">
            <h1>Smart Helmet Dashboard</h1>
            <div class="card">
              <h2>Temperature</h2>
              <p class="status"><span id="temperature">Loading...</span></p>
            </div>
            <div class="card">
              <h2>Humidity</h2>
              <p class="status"><span id="humidity">Loading...</span></p>
            </div>
            <div class="card">
              <h2>Fall Detected</h2>
              <p class="status"><span id="fallStatus" class="fall-safe">Loading...</span></p>
            </div>
            <div class="card">
              <h2>Gas Detected</h2>
              <p class="status"><span id="gasStatus" class="gas-safe">Loading...</span></p>
              <p>Gas Concentration: <span id="gasConcentration">Loading...</span></p>
            </div>
            <div class="card">
             <h2>Emergency SOS</h2>
             <p class="status"><button onclick="sendSOS()">Activate SOS</button></p>
            </div>
            <script>
             function sendSOS() {
              fetch('/sos')
               .then(response => {
                 if (response.ok) {
                  alert('SOS activated!');
                 } else {
                   alert('Failed to activate SOS.');
                 }
               })
               .catch(error => console.error('Error:', error));
             }
             </script>
          </div>
        </body>
        </html>
      )rawliteral");
    }
    delay(1);
    client.stop();
  }

  if (digitalRead(buzzerPin) == HIGH) {
    unsigned long currentMillis = millis();

    // Check if the specified time has passed
    if (currentMillis - previousMillis >= delayInterval) {
      previousMillis = currentMillis;  // Update the last time the message was sent

      // Send the LoRa message
      LoRa.beginPacket();
      LoRa.print(F("Helmet 01 Needs Help"));
      LoRa.print(json);
      LoRa.endPacket();

      Serial.println("Data sent via LoRa:");
    }
  }
}
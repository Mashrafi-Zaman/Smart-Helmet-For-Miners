#include <SPI.h>
#include <LoRa.h>

#define SS_PIN    10  // Chip Select Pin (NSS)
#define RESET_PIN 9   // Reset Pin
#define DIO0_PIN  2   // DIO0 Pin

void setup() {
  // Start serial communication
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  // Initialize LoRa module
  if (!LoRa.begin(433E6)) {  // Frequency for Europe (433 MHz), adjust if necessary
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Set the LoRa module's parameters (optional)
  LoRa.setSpreadingFactor(7);  // Adjust spreading factor
  LoRa.setSignalBandwidth(125E3);  // Set bandwidth to 125 kHz
  LoRa.setCodingRate4(5);  // Set coding rate to 4/5

  Serial.println("LoRa Initializing...");
}

void loop() {
  // Check if there is incoming LoRa packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet: ");
    
    // Read the incoming packet
    while (LoRa.available()) {
      String incoming = LoRa.readString();
      Serial.print(incoming);
    }
    Serial.println();
  }
}
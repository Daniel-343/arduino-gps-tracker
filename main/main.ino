// VehicleTracker.ino
#include "Init.h"
#include "NetworkManager.h"
#include "Gps.h"

const long interval = 1000 * 60 * 5;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  
  while (!initializeHardware()) {
    Serial.println("Hardware init failed. Retrying in 5...");
    delay(5000); 
  }

  while (!connectCellular()) {
    Serial.println("Connection to cellular network failed. Retrying in 5...");
    delay(5000); 
  }
}

void loop() {
  
  if (!isCellularConnected()) {
    connectCellular();
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendLocationUpdate();
  }

  delay(10000); 
}
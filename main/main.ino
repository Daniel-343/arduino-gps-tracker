// VehicleTracker.ino
#include "Init.h"
#include "NetworkManager.h"
#include "Gps.h"
#include <Arduino_MKRGPS.h>

const long interval = 10000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  unsigned long startWait = millis();
  while (!Serial && millis() - startWait < 5000);

  analogReference(AR_DEFAULT);
  analogReadResolution(12);
  initGPSModule();
  while (!initializeHardware()) {
    Serial.println("Hardware init failed. Retrying in 5...");
    delay(5000); 
  }
  Serial.println("All hardware initialized successfully");
  setCurrentTime();
}

void loop() {

  
  if (!isCellularConnected()) {
    connectCellular();
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    getCurrentTime();
    getBatteryStatus();
    sendLocationUpdate();
  }
}
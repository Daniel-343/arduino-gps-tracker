#include "USB/USBAPI.h"
#include "Init.h"
#include <MKRNB.h>
#include <Arduino_MKRGPS.h>
#include "NetworkManager.h"
#include "Gps.h"

bool initializeHardware() {
  bool success = true;
  Serial.println("Starting initialization sequence...");
  Serial.println("-----------------------------------");

  //if(initGPSModule()){
  //  Serial.println("GPS init successful");
  //}
  //else{
    //Serial.println("GPS init failed");
   // success = false;
  //}
  if(!connectCellular()){
    Serial.println("connection failed");
    success = false;
  }

  return success;
}

void getBatteryStatus(){
  int rawADC = analogRead(ADC_BATTERY);

  // 2. Calculate the voltage at the ADC pin
  // ADC reads between 0 and 3.3V across 4096 possible values
  float voltADC = rawADC * (3.3 / 4095.0);

  // 3. Calculate the actual battery voltage using the 1.275 multiplier
  float voltBat = voltADC * 1.275;

  // 4. Calculate battery percentage
  // A standard LiPo is ~3.2V when empty and 4.2V when full
  float maxVoltage = 4.2;
  float minVoltage = 3.2;
  
  // Map the voltage to a 0-100 scale
  int batteryPercentage = ((voltBat - minVoltage) / (maxVoltage - minVoltage)) * 100;
  
  // Constrain the percentage to 0-100% just in case of slight voltage spikes/drops
  batteryPercentage = constrain(batteryPercentage, 0, 100);

  // 5. Display the data
  Serial.print("Raw ADC: ");
  Serial.print(rawADC);
  Serial.print("  |  ADC Pin Voltage: ");
  Serial.print(voltADC);
  Serial.print("V  |  Real Battery Voltage: ");
  Serial.print(voltBat);
  Serial.print("V  |  Percentage: ");
  Serial.print(batteryPercentage);
  Serial.println("%");
}
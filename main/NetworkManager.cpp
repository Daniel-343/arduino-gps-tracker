#include "NetworkManager.h"
#include "USB/USBAPI.h"
#include <MKRNB.h>
#include <Arduino_MKRGPS.h>
#include <RTCZero.h>
#include <NTPClient.h>

NB nbAccess;
GPRS gprs;
NBScanner scanner;
NBUDP Udp;

NTPClient timeClient(Udp);
RTCZero rtc;

const char PINNUMBER[] = ""; 
const char APN[] = "iot.secure";
const char USERNAME[] = "";
const char PASSWORD[] = "";

bool connectCellular() {
  Serial.println("Connecting to cellular network...");

  while (!((nbAccess.begin(PINNUMBER, APN, USERNAME, PASSWORD) == NB_READY) && 
      (gprs.attachGPRS() == GPRS_READY))) {
    Serial.print(".");
    delay(2000);
  }
  
  Serial.println("\nSuccessfully connected to the network!");
  
  bool connectionSuccessful = true; 
  
  return connectionSuccessful;
}

bool isCellularConnected() {
  String signalStr = scanner.getSignalStrength();
  int signalStrength = signalStr.toInt();
  
  if (signalStrength == 0 || signalStrength == 99) {
    Serial.println("Cellular signal lost.");
    return false; 
  }
  return true;
}

void setCurrentTime(){
  rtc.begin();
  
  // Start the NTP client
  timeClient.begin();
  
  // Set your time zone offset in seconds. 
  // e.g., GMT +1 = 3600, GMT +8 = 28800, GMT -4 = -14400
  timeClient.setTimeOffset(0); 

Serial.print("Fetching NTP time");
  int attempts = 0;
  
  // Try up to 10 times, waiting 1 second between each try
  while(!timeClient.update() && attempts < 10) {
    timeClient.forceUpdate();
    delay(1000); // Crucial: Give the modem time to actually send/receive!
    Serial.print(".");
    attempts++;
  }

  if (attempts < 10) {
    // We got the time!
    rtc.setEpoch(timeClient.getEpochTime());
    Serial.println("\nRTC successfully set!");
  } else {
    // It failed, but we safely move on without crashing
    Serial.println("\nNTP request timed out. Moving on...");
  }
}
void getCurrentTime(){
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());
  Serial.println();
  }

void print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}

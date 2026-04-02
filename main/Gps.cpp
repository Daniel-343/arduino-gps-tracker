#include "GPS.h"

float getLatitude() {
  return 0; 
}

float getLongitude() {
  return 0;
}

void sendLocationUpdate() {
  float lat = getLatitude();
  float lon = getLongitude();
  
  Serial.print("Formatting data for Lat: ");
  Serial.print(lat);
  Serial.print(" Lon: ");
  Serial.println(lon);

  
  String url = "http://YOUR_TRACCAR_SERVER:5055/?id=YOUR_DEVICE_ID";
  url += "&lat=" + String(lat, 6);
  url += "&lon=" + String(lon, 6);
  
  Serial.println("Location sent to Traccar.");
}
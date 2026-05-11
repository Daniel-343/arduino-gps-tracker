#include "GPS.h"
#include <Arduino_MKRGPS.h>
#include <Arduino.h>
#include <MKRNB.h>

NBClient client;

const char server[] = "demo3.traccar.org";
const int port = 5055;
const String deviceId = "69"; 
unsigned long lastNetworkSend = 0;
float getLatitude() {
  return 0; 
}

float getLongitude() {
  return 0;
}

// Add this global variable at the top of your file near your others
unsigned long lastGpsCheck = 0;

void sendLocationUpdate() {
  // ONLY run this function once per second. Otherwise, exit immediately.
  if (millis() - lastGpsCheck < 1000) {
    return; 
  }
  lastGpsCheck = millis(); // Reset the timer

  if (GPS.available()) {
    
    Serial.println(); 
    Serial.println("LOCK ACQUIRED!");
    
    float latitude   = GPS.latitude();
    float longitude  = GPS.longitude();
    float altitude   = GPS.altitude();
    float speed      = GPS.speed();
    int   satellites = GPS.satellites();

    Serial.print("Location: ");
    Serial.print(latitude, 7);
    Serial.print(", ");
    Serial.println(longitude, 7);
    Serial.print("Number of satellites: ");
    Serial.println(satellites);
    Serial.println();

    sendLocationToTraccar(latitude, longitude);
    flickerInternalLED();
    
  } else {
    // Now it will only print this once per second, safely.
    Serial.println("unable to get a satellite lock.");
  }
}

void sendLocationToTraccar(float lat, float lon) {
  Serial.print("Connecting to Traccar server... ");
  
  // Connect to the server
  if (client.connect(server, port)) {
    Serial.println("Connected!");
    
    // Construct the HTTP GET request
    // Format: GET /?id=69&lat=46.123&lon=18.456 HTTP/1.1
    client.print("GET /?id=");
    client.print(deviceId);
    client.print("&lat=");
    client.print(lat, 6); // Send latitude with 6 decimal places
    client.print("&lon=");
    client.print(lon, 6); // Send longitude with 6 decimal places
    client.println(" HTTP/1.1");
    
    // Standard HTTP headers
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println(); // Blank line indicates the end of the request
    
    Serial.println("Location data sent.");
    
    // Read the response from the server (Optional, but good for debugging)
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    
    // Disconnect
    client.stop();
    Serial.println("\nDisconnected from server.");
    
  } else {
    // If you didn't get a connection to the server
    Serial.println("Connection failed.");
  }
}

bool initGPSModule() {
  if (GPS.begin()) {
    return true;
  } else {
    return false;
  }
}

void flickerInternalLED() {
  unsigned long startTime = millis();
  const unsigned long duration = 5000; // 5 seconds in milliseconds
  int flickerSpeed = 200; // Speed of the flicker in milliseconds

  while (millis() - startTime < duration) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(flickerSpeed);
    digitalWrite(LED_BUILTIN, LOW);
    delay(flickerSpeed);
  }
  
  digitalWrite(LED_BUILTIN, LOW);
}
#include <Arduino_MKRGPS.h>
#include <MKRNB.h>

NB nbAccess;
GPRS gprs;
NBClient client;
NBScanner scanner;

// Back to your exact original settings
const char server[] = "demo3.traccar.org";
const int port = 5055;
const String deviceId = "69"; 

const char PINNUMBER[] = ""; 
const char APN[] = "iot.secure";
const char USERNAME[] = "";
const char PASSWORD[] = "";

unsigned long lastNetworkSend = 0;

void setup() {
  Serial.begin(115200);
  
  unsigned long startWait = millis();
  while (!Serial && millis() - startWait < 5000); 
  flickerInternalLED();
  initializeGPS();
  initializeCellular();
}

void loop() {
  if (GPS.available()) {
    
    // Check every 10 seconds (fast enough to keep the modem awake)
    if (millis() - lastNetworkSend > 10000) {
      
      float latitude   = GPS.latitude();
      float longitude  = GPS.longitude();
      int   satellites = GPS.satellites();

      Serial.println("\n--- GPS Lock Active ---");
      Serial.print("Satellites: "); Serial.println(satellites);
      Serial.print("Location: ");
      Serial.print(latitude, 7);
      Serial.print(", ");
      Serial.println(longitude, 7);
      
      sendLocationToTraccar(latitude, longitude);
      
      lastNetworkSend = millis(); 
    }
  }
}

void initializeGPS() {
  Serial.println("Initializing GPS over I2C...");
  if (!GPS.begin()) {
    Serial.println("Failed to initialize GPS!");
    while (1); 
  }
  Serial.println("GPS initialized!");
}

void initializeCellular() {
  Serial.println("Connecting to cellular network...");

  while (!((nbAccess.begin(PINNUMBER, APN, USERNAME, PASSWORD) == NB_READY) && 
      (gprs.attachGPRS() == GPRS_READY))) {
    Serial.print(".");
    delay(2000);
  }
  
  Serial.println("\nSuccessfully connected to the network!");
}

// Your EXACT original function that worked previously
void sendLocationToTraccar(float lat, float lon) {
  Serial.print("Connecting to Traccar server... ");
  
  if (client.connect(server, port)) {
    Serial.println("Connected!");
    
    client.print("GET /?id=");
    client.print(deviceId);
    client.print("&lat=");
    client.print(lat, 6); 
    client.print("&lon=");
    client.print(lon, 6); 
    client.println(" HTTP/1.1");
    
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println(); 
    
    Serial.println("Location data sent.");
    
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    
    client.stop();
    Serial.println("\nDisconnected from server.");
    flickerInternalLED();
    
  } else {
    Serial.println("Connection failed.");
  }
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
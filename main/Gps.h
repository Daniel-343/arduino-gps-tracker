#ifndef GPS_H
#define GPS_H

#include <Arduino.h>

void sendLocationUpdate();
bool initGPSModule();
void sendLocationToTraccar(float lat, float lon);
void flickerInternalLED();

#endif
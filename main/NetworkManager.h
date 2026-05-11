#ifndef CELLULAR_H
#define CELLULAR_H

#include <Arduino.h>

bool connectCellular();
bool isCellularConnected();
void setCurrentTime();
void getCurrentTime();
void print2digits(int number);

#endif
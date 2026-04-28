#ifndef SCD_SYSTEM_H
#define SCD_SYSTEM_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_SCD4x_Arduino_Library.h"

//#define NUM_OF_SENSORS 2

struct SCDpackage
{
  SCD4x scdSensor;
  bool status;
  bool isValid;
};

// Function declarations
void selectMuxChannel(uint8_t channel);
void initSCDs(SCDpackage packages[], const uint8_t channels[], int numSensors);
bool initSensorOnChannel(SCD4x &sensor, uint8_t channel, int index);
void sendINIT(int sensorNum, bool status);
void sendDataStream(bool dataStreamOn);
void readSCDs(SCDpackage packages[], const uint8_t channels[], int numSensors);


#endif

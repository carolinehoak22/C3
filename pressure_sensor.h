#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include <Arduino.h>

//#define NUM_OF_SENSORS 2 //number of sensor packages

struct PressureSensor
{
  int analogValue;
  float absolutePSI;
  float gaugePSI; //conversion using voltage
  float baselineVoltage;
  float voltage;
  float PSI; //another conversion
  float gaugeKPa;
  float absoluteKPa;
};

void initPressure(PressureSensor sensors[], int numSensors);
void readPressureLoop(PressureSensor sensors[], int numSensors);

#endif
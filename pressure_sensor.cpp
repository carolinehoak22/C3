#include "pressure_sensor.h"

//Presure Configuration==================
const int pressurePins[] = {A1, A2, A3, A4, A5}; //analog pins for pressure sensors
const double baselineVoltage[] = {0.511, 0.500, 0.504, 0.531, 0.511}; //pressure baseline voltage setup

const float ambientPressurePSI = 14.8; //according to tides4Fishing.com

double pressureMin = 102.4;  //0 psi = 0.5v
double pressureMax = 921.6;  //150 psi = 4.5v
double pressurePSImin = 0.0; // psi 
double pressurePSImax = 80.0; // psi

const float PSI2kPa = 6.89476; 
const float AtmoKPa = 101.3;  


//======Functions=================
void initPressure(PressureSensor sensors[], int numSensors)
{
  for(int i = 0; i < numSensors; i++)
  {
    sensors[i].baselineVoltage = baselineVoltage[i];
  }
}

void readPressureLoop(PressureSensor sensors[], int numSensors)
{
  for(int i = 0; i < numSensors; i++)
  {
    sensors[i].analogValue = analogRead(pressurePins[i]);
    sensors[i].voltage = sensors[i].analogValue * (5.0/1023.0);
    sensors[i].gaugePSI = (sensors[i].voltage - sensors[i].baselineVoltage) * (pressurePSImax / (4.5 - sensors[i].baselineVoltage));
    sensors[i].absolutePSI = sensors[i].gaugePSI + ambientPressurePSI;
    sensors[i].PSI = ((sensors[i].analogValue - pressureMin)*pressurePSImax)/(pressureMax-pressureMin); 
    sensors[i].gaugeKPa = sensors[i].gaugePSI * PSI2kPa;
    sensors[i].absoluteKPa = sensors[i].gaugeKPa + AtmoKPa;
  }
}
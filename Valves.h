#ifndef VALVES_H
#define VALVES_H

#include <Arduino.h>
//#define NUM_VALVES 5
#define ON true
#define OFF false

struct ValveStruct
{
  int valvePin;
  bool valveState;  //true for on, false for off
};

// Function declarations
void initValvePins(ValveStruct valves[], int numSensors);
void closeValve(ValveStruct &valve);
void openValve(ValveStruct &valve);
void turnAllValvesON(ValveStruct valves[], int numSensors);
void turnAllValvesOFF(ValveStruct valves[], int numSensors);
void changeValveState(ValveStruct &valve, bool valveState);
void sendValveStates(ValveStruct valves[], int numValves);
void getValveState(ValveStruct &valve, int valveNum);

#endif
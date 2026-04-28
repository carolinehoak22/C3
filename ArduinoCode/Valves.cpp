/*==============================

===============================*/

#include "Valves.h"
int valvePins[] = {32,31,30,33,34,47}; //make changeable

void initValvePins(ValveStruct valves[], int numValves)
{
  //const int numValves = sizeof(valves)/sizeof(valves[0]);
  for(int i = 0; i < numValves; i++)
  {
    valves[i].valvePin = valvePins[i];
    pinMode(valves[i].valvePin, OUTPUT);
  }
}

void valveON(ValveStruct &valve)
{
  digitalWrite(valve.valvePin, HIGH);
  valve.valveState = ON;
}

void valveOFF(ValveStruct &valve)
{
  digitalWrite(valve.valvePin, LOW);
  valve.valveState = OFF;
}

void turnAllValvesON(ValveStruct valves[], int numValves)
{
  for(int i = 0; i < numValves; i++)
  {
    valveON(valves[i]);
  }
}

void turnAllValvesOFF(ValveStruct valves[], int numValves)
{
  for(int i = 0; i < numValves; i++)
  {
    valveOFF(valves[i]);
  }
}

void changeValveState(ValveStruct &valve, bool valveState)
{
  if(valveState)
  {
    valveON(valve);
  }else{
    valveOFF(valve);
  }
}

// ex. <VALVE,1,ON>
void sendValveStates(ValveStruct valves[], int numValves)
{
  for(int i = 0; i < numValves; i++)
  {
    getValveState(valves[i],i);
  }
}

void getValveState(ValveStruct &valve, int valveNum)
{
  Serial.print("<VALVE,");
  Serial.print(valveNum + 1);
  Serial.print(",");
  if(valve.valveState)
  {
    Serial.print("ON");
  }else
  {
    Serial.print("OFF");
  }
  Serial.println(">");
}

void startingValveStates()
{
  
}

/*===============================================
This is the full_system for the C3 system.

Created by Marion Bean. Last updated 04/09/2026.
============================================*/

#include <Wire.h>
#include "pressure_sensor.h"
#include "SCD_MUX.h"
#include "Valves.h"
#include "thermocouple.h"

#define NUM_OF_SENSORS 5 //number of sensor packages, includes SCD+pressure
#define NUM_OF_VALVES 6  //valve 6 controls the pump

bool dataStreamOn = false;

//=============Thermocouple======================
Adafruit_MCP9600 mcp;
float thermocoupleTemp = 0.0;

//=============Pressure=========================

PressureSensor PS_package[NUM_OF_SENSORS];  //pressure sensor package

//=============SCD41 Sensor=====================
const uint8_t MUX_Channels[] = {1,2,3,4,5};
SCDpackage SCD_package[NUM_OF_SENSORS];

//=============Valves======================
ValveStruct valves[NUM_OF_VALVES];

//============Serial handling===============
String inputBuffer = "";

//==================SETUP====================
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
   Serial.println(F("================================================="));
  Serial.println(F("                FULL C3 SYSTEM                      "));
  Serial.println(F("================================================="));
  Wire.begin();

  //initially data stream is off until it is turned on through labview (serial monitor)
  dataStreamOn = false; 

  //Check if thermocouple is connected---------------
  initMCP(mcp);

  //Check if SCD sensors are connected----------------
  initSCDs(SCD_package, MUX_Channels, NUM_OF_SENSORS);

  //Initialize Pressure Sensor------------------------
  initPressure(PS_package, NUM_OF_SENSORS);

  //Initiaize valve pins==============
  initValvePins(valves, NUM_OF_VALVES);

  //TEMPORARY
  //startingValveStates();

  Serial.println("------------------------------------------------------");
  Serial.println("<PACKET_TYPE, Time (s), Sensor Package #, CO2 (ppm), Temp (C), Humid (%), Pressure (KPa), Thermocouple Temp (C)>");
  delay(1000);

}

//==================LOOP=============================
void loop() 
{
  
  handleSerialCommands();
  delay(100);
  if(dataStreamOn)
  {
    static unsigned long lastPrint = 0;
    unsigned long now = millis();
    if (now - lastPrint >= 5000) 
    {
      lastPrint = now;
      
      readSCDs(SCD_package, MUX_Channels, NUM_OF_SENSORS);
      readPressureLoop(PS_package, NUM_OF_SENSORS);
      thermocoupleTemp = readMCP(mcp);

      unsigned long seconds = now / 1000;

      sendDataPackets(seconds);
    }
  }
  delay(100);

}

//===============FUNCTIONS=================================

void sendDataPackets(unsigned long sec)
{
  for(int i = 0; i < NUM_OF_SENSORS; i++)
  {
    Serial.print("<DATA,");

    Serial.print(sec);
    Serial.print(",");

    Serial.print(i+1);
    Serial.print(",");
    
    if(SCD_package[i].isValid)
    {
      Serial.print(SCD_package[i].scdSensor.getCO2());
      Serial.print(",");

      Serial.print(SCD_package[i].scdSensor.getTemperature());
      Serial.print(",");

      Serial.print(SCD_package[i].scdSensor.getHumidity());
      Serial.print(",");

      Serial.print(PS_package[i].absoluteKPa, 2);
      Serial.print(",");

      Serial.print(thermocoupleTemp);
  
    }else{
      Serial.print("0,0,0,0,0");
    }

    Serial.println(">");
  }
}

void sendErrorMSG(String msg)
{
  Serial.print("<ERROR, ");
  Serial.print(msg);
  Serial.println(">");
}


void handleSerialCommands()
{
  while (Serial.available())
  {
    char c = Serial.read();    

    // Parse through each character in the command until newline, then process command
    if (c == '\n')
    {
      inputBuffer.trim();
      inputBuffer.toUpperCase();

      processSerialCommand(inputBuffer);
      inputBuffer = ""; // clear buffer
    }
    else
    {
      inputBuffer += c; // build command
    }
  }
}

//--------COMMANDS TO BE SENT BY LABVIEW (or any serial communicator)-----------------
void processSerialCommand(String input)
{
  if(input.length() == 0) return; //if nothing is inputed then nothing happens
  
  //--------Control data flow------
  if(input == "STREAM_ON")
  {
    dataStreamOn = true;
    sendDataStream(dataStreamOn);
    return;
  }

  if(input == "STREAM_OFF")
  {
    dataStreamOn = false;
    sendDataStream(dataStreamOn);
    return;
  }

  //--------Check Valve Status------
  if(input == "VALVE_STATES")
  {
    sendValveStates(valves, NUM_OF_VALVES);
    return;
  }


  //------ALL VAlVE COMMANDS---------
  if(input == "ALL OFF")
  {
    turnAllValvesOFF(valves, NUM_OF_VALVES);
    sendValveStates(valves, NUM_OF_VALVES);
    return;
  }

  if(input == "ALL ON")
  {
    turnAllValvesON(valves, NUM_OF_VALVES);
    sendValveStates(valves, NUM_OF_VALVES);
    return;
  }

  //-----INDIVIDUAL VALVE COMMANDS-------------------------
  // Ex. V1 ON or V1 OFF
  if(input.startsWith("V"))
  { 
    if(NUM_OF_VALVES == 0)
    {
      sendErrorMSG("No valves configured");
      return;
    }

    if(input.length() < 4)
    {
      sendErrorMSG("Invalid command format");
      return;
    }

    int valveNum = input.charAt(1) - '0';  //convert the character at position 1 to an integer
    if(valveNum < 0 || valveNum > NUM_OF_VALVES)
    {
      sendErrorMSG("Invalid valve number");
      return;
    }

    String valveStateStr = input.substring(3);
    bool valveState;

    if(valveStateStr == "ON")
    {
      valveState = true;
    }else if(valveStateStr == "OFF")
    {
      valveState = false;
    }else
    {
      sendErrorMSG("Invalid valve state");
      return;
    }

    changeValveState(valves[valveNum - 1], valveState);
    sendValveStates(valves, NUM_OF_VALVES);

    return;
  }

  sendErrorMSG("Unknown command");
  
}

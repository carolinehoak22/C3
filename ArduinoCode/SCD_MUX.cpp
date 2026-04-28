#include "SCD_MUX.h"

//=============Config=================
#define MUX_ADDR 0x70
#define SCD41_ADDR 0x62



void selectMuxChannel(uint8_t channel) 
{
  if (channel > 7) return;  
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << channel); 
  Wire.endTransmission();
  delay(10); 
}

//initialize all SCD sensors
void initSCDs(SCDpackage packages[], const uint8_t channels[], int numSensors)
{
  
  for(int i = 0; i < numSensors; i++)
  {
    initSensorOnChannel(packages[i].scdSensor, channels[i], i);
  }
}

// initialize/check one SCD sensor
bool initSensorOnChannel(SCD4x &sensor, uint8_t channel, int index) 
{
  selectMuxChannel(channel);
  if (sensor.begin() == false) 
  {
    //Serial.println(" not detected. Check wiring!");
    sendINIT(index, false);
    return false;
  }

  if (sensor.startPeriodicMeasurement() == false) 
  {
    //Serial.println(" failed to start measurements");
    sendINIT(index, false);
    return false;
  }
  //sensor detected, send message
  sendINIT(index, true);
  return true;
}

//send intialization packet to labview
void sendINIT(int sensorNum, bool status)
{
  Serial.print("<INIT,SCD,");
  Serial.print(sensorNum + 1);
  Serial.print(",");
  
  if(status)
  {
    Serial.print("OK");
  }
  else{
    Serial.print("ERROR");
  }
  Serial.println(">");
}

void sendDataStream(bool dataStreamOn)
{
  Serial.print("<MSG,");
  if(dataStreamOn)
  {
    Serial.print("Collecting Data...");
  }else{
    Serial.print("Data Collection OFF");
  }

  Serial.println(">");
}


//read all SCD sensors
void readSCDs(SCDpackage packages[], const uint8_t MUX_Channels[], int numSensors)
{
  for(int i = 0; i < numSensors; i++)
  {
    selectMuxChannel(MUX_Channels[i]);
    delay(5);
    if(packages[i].scdSensor.readMeasurement())
    {
      packages[i].isValid = true;
    }
    else
    {
      packages[i].isValid = false;
    }
  }
}



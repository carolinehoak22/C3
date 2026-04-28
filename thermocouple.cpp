#include "thermocouple.h"

bool mcpStatus = false;

void initMCP(Adafruit_MCP9600 &mcp)
{
  mcpStatus = false;
  if (mcp.begin()) 
  {
    mcpStatus = true; 
  }
  sendINIT();

  //Serial.println("MCP9600 Found!");
  mcp.setThermocoupleType(MCP9600_TYPE_T);
}

void sendINIT()
{
  Serial.print("<INIT,MCP,");
  
  if(mcpStatus)
  {
    Serial.print("OK");
  }
  else{
    Serial.print("ERROR");
  }
  Serial.println(">");
}

float readMCP(Adafruit_MCP9600 &mcp)
{
  if(!mcpStatus)
  {
    return 0.0;
  }
  return mcp.readThermocouple();
}
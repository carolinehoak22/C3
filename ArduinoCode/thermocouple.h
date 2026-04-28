#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP9600.h>




void initMCP(Adafruit_MCP9600 &mcp);
void sendINIT();
float readMCP(Adafruit_MCP9600 &mcp);

#endif

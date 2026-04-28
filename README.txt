Libraries needed to download (ctrl + shift + i):
->SparkFun_SCD4x_Arduino_Library
->Adafruit MCP9600
download all dependencies

Make sure Arduino board settings are correct
->Board: Arduino Mega or Mega 2560
->COM#: should automatically come up or say board name next to it
	->can also check the computers 'Device Manager'

Commands to send:
STREAM_ON
STREAM_OFF
VALVE_STATES
ALL ON
ALL OFF
V# ON   (# corresponds to valve number)
V# OFF

Data Format
<PACKET_TYPE, Time (s), Sensor Package #, CO2 (ppm), Temp (C), Humid (%), Pressure (KPa), Thermocouple Temp (C)>

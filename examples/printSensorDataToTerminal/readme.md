
# Arduino library for the HyQuant Q sensor

## Communikation:

In order to receive the sensor information it needs to be requested first over the SDI12 bus.
SDI12 command structure: [adress][command]!

Command flow to get all the relevant data: (x variable for programmable device adress)
  1. xM! : start first measurement
  -> Sensor sends back status if it's ready or not, only continue when ready

  2. xD0! : request data from first measurement
  -> Sensor sends back string in the following format: a+d+d+d+d+d

  3. xM1! : start second measurement
  -> Sensor sends back status if it's ready or not, only continue when ready

  4. xD0! : request data from second measurement
  -> Sensor sends back string in the following format: a+d+d+d

  4. xM2! : start third measurement
  -> Sensor sends back status if it's ready or not, only continue when ready

  5. xD0! : request data from third measurement
  -> Sensor sends back string in the following format: a+d+d+d+d+d

## Sensor data:
Ffter each measurement the sensor returns the data in the following format: a+d+d+d+d+d
a is a wildcard for the sensor adress and the d's stand for floating point or uint16 numbers.

Data response for command "xM!": 0+1+2+3+4+5
0   uint16_t sensorAdress1;
1   float filteredLevel;
2   float filteredVelo;
3   uint16_t SNRlevel;
4   uint16_t SNRvelo;
5   float discharge;

Data response for command "xM1!": 6+7+8+9+10+11+12
6   uint16_t sensorAdress2;
7   float currentLevel;
8   float currentDistance;
9   float  currentVelo;
10  uint16_t qualityLevel;
11  uint16_t qualityVelo;
12  uint16_t qualitydischarge;

Data response for command "xM2!"
13  uint16_t sensorAdress3;
14  float temp;
15  float tiltX;
16  float tiltY;
17  float heightAboveRef;

The length of the response is dependent on the measurement. Numbers can also be negative, in this case they replace the delimiting "+" with an "-".
Some examples from valid sensor responses:
0-10+5.31+4-2.22
0+99+5.31+4-2.22+222+89.99

## Problems:
With my code i could onyl use this funktion in order not to receive just data garbage: "mySDI12.readStringUntil('\n');"
The problem is that this function only returns an arduino String which is subsceptible to RAM-fragmentation.

Better way to do things would be to use the "mySDI12.available();" function, but with this i could not receive consistent data









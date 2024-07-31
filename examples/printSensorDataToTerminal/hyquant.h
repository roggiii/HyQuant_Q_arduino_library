/*
Copyright (c) <2024>, <AlpGeotech>
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree. 
*/

#ifndef HYQUANT_H_
#define HYQUANT_H_

#include "Arduino.h"
#include "RAK13010_SDI12.h"  // Click to install library: http://librarymanager/All#RAK13010

#define SENSOR_ADDRESS '0'

// Data structure for response to command "xM!"
struct HyQuant_Data {
  uint16_t sensorAdress1;
  float filteredLevel;
  float filteredVelo;
  uint16_t SNRlevel;
  uint16_t SNRvelo;
  float discharge;

  uint16_t sensorAdress2;
  float currentLevel;
  float currentDistance;
  float  currentVelo;
  uint16_t qualityLevel;
  uint16_t qualityVelo;
  uint16_t qualitydischarge;

  uint16_t sensorAdress3;
  float temp;
  float tiltX;
  float tiltY;
  float heightAboveRef;
};

// checks if sensor is online
bool isOnline(RAK_SDI12& mySDI12);

// requests data from the sensor
bool getParameters(RAK_SDI12& mySDI12, HyQuant_Data& SensorData);

// in an error condition set all sensor parameters to zero
void setAllSensorDataZero(HyQuant_Data& SensorData);


// String mangeling
void writeSensorDataToStruct(HyQuant_Data& SensorData);
String insertSpaceBeforeMinus(const String& input);
String replacePlusWithSpace(const String& input);
String getValue(String data, char separator, int index);

bool isAllZerosExceptLast(const String& str);





#endif // HYQUANT_H_
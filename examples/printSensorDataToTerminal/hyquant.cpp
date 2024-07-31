/*
Copyright (c) <2024>, <AlpGeotech>
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree. 
*/
#include "hyquant.h"  //  Header file for this library

String rawSensorResponse;

void writeSensorDataToStruct(HyQuant_Data& SensorData)
{
  SensorData.sensorAdress1    = getValue(rawSensorResponse, ' ', 0).toInt();
  SensorData.filteredLevel    = getValue(rawSensorResponse, ' ', 1).toFloat();
  SensorData.filteredVelo     = getValue(rawSensorResponse, ' ', 2).toFloat();
  SensorData.SNRlevel         = getValue(rawSensorResponse, ' ', 3).toInt();
  SensorData.SNRvelo          = getValue(rawSensorResponse, ' ', 4).toInt();
  SensorData.discharge        = getValue(rawSensorResponse, ' ', 5).toFloat();

  SensorData.sensorAdress2    = getValue(rawSensorResponse, ' ', 6).toInt();
  SensorData.currentLevel     = getValue(rawSensorResponse, ' ', 7).toFloat();
  SensorData.currentDistance  = getValue(rawSensorResponse, ' ', 8).toFloat();
  SensorData.currentVelo      = getValue(rawSensorResponse, ' ', 9).toFloat();
  SensorData.qualityLevel     = getValue(rawSensorResponse, ' ', 10).toInt();
  SensorData.qualityVelo      = getValue(rawSensorResponse, ' ', 11).toInt();
  SensorData.qualitydischarge = getValue(rawSensorResponse, ' ', 12).toInt();

  SensorData.sensorAdress3    = getValue(rawSensorResponse, ' ', 13).toInt();
  SensorData.temp             = getValue(rawSensorResponse, ' ', 14).toFloat();
  SensorData.tiltY            = getValue(rawSensorResponse, ' ', 15).toFloat();
  SensorData.tiltY            = getValue(rawSensorResponse, ' ', 16).toFloat();
  SensorData.heightAboveRef   = getValue(rawSensorResponse, ' ', 17).toFloat();
}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


bool isAllZerosExceptLast(const String& str) {
  // Check if the string has exactly 5 characters
  if (str.length() != 5) {
    return false;  // Not the expected length
  }

  // Check if the last character is not '0'
  if (str.charAt(4) == '0') {
    return false;  // Last character should not be '0'
  }

  // Check that all characters except the last are '0'
  for (int i = 0; i < 4; i++) {
    if (str.charAt(i) != '0') {
      return false;  // Found a non-'0' character before the last position
    }
  }

  return true;  // All conditions met
}

void setAllSensorDataZero(HyQuant_Data& SensorData)
{
  SensorData.sensorAdress1    = 0;
  SensorData.filteredLevel    = 0;
  SensorData.filteredVelo     = 0;
  SensorData.SNRlevel         = 0;
  SensorData.SNRvelo          = 0;
  SensorData.discharge        = 0;

  SensorData.sensorAdress2    = 0;
  SensorData.currentLevel     = 0;
  SensorData.currentDistance  = 0;
  SensorData.currentVelo      = 0;
  SensorData.qualityLevel     = 0;
  SensorData.qualityVelo      = 0;
  SensorData.qualitydischarge = 0;

  SensorData.sensorAdress3    = 0;
  SensorData.temp             = 0;
  SensorData.tiltY            = 0;
  SensorData.tiltY            = 0;
  SensorData.heightAboveRef   = 0;
}

bool isOnline(RAK_SDI12& mySDI12)
{
  mySDI12.sendCommand(String(SENSOR_ADDRESS) + "!");
    static String sdiResponse;
    sdiResponse = mySDI12.readStringUntil('\n');
    delay(100);
    sdiResponse.trim();
    if(sdiResponse == String(SENSOR_ADDRESS)){
      return true;
    }
    else
    {
      Serial.println("sensor offline");
      return false;
    }
}

// read all the sensor parameters: 0M! 0M1! 0M2!
bool getParameters(RAK_SDI12& mySDI12, HyQuant_Data& SensorData){

  static String sdiResponse;
  static String sdiResponse1;
  static String sdiResponse2;
  static String sdiResponse3;

  // checking if sensor is running
  if(isOnline(mySDI12) == false)

    // Sensor not running ... setting all data to zero
    setAllSensorDataZero(SensorData);
    return 0;

  //three Data sets need to be collected
  for (int i = 0; i <= 2; i++)
  {

    switch(i){
      case 0:
        //Serial.println("Requesting data ... "+ String(SENSOR_ADDRESS) + "M!");
        mySDI12.sendCommand(String(SENSOR_ADDRESS) + "M!"); break;
      case 1:
        //Serial.println("Requesting data ... "+ String(SENSOR_ADDRESS) + "M1!");
        mySDI12.sendCommand(String(SENSOR_ADDRESS) + "M1!"); break;
      case 2:
        //Serial.println("Requesting data ... "+ String(SENSOR_ADDRESS) + "M2!");
        mySDI12.sendCommand(String(SENSOR_ADDRESS) + "M2!"); break;
    }
      
    sdiResponse = mySDI12.readStringUntil('\n');
    delay(100);

    //newline und line carrige entfernen
    sdiResponse.trim();

    //wenn alle Charaktere in Antwort 0 sind bis auf den letzten heißt das die Daten sofort zum abholen bereit sind
    // 0000X <- nur der letzte Charakter ist ein Wildcard
    if(isAllZerosExceptLast(sdiResponse))
    {
      //Send command to request data from the sensor
      mySDI12.sendCommand(String(SENSOR_ADDRESS) + "D0!");

      sdiResponse = mySDI12.readStringUntil('\n');
      delay(100);

      switch(i){
        case 0: sdiResponse1 = sdiResponse; break;
        case 1: sdiResponse2 = sdiResponse; break;
        case 2: sdiResponse3 = sdiResponse; break;
      }

    }else{
      Serial.println("Sensor is still processing data, waiting ...");

      // Sensor processing ... setting all data to zero
      setAllSensorDataZero(SensorData);
      return 0;
    }
  }
  sdiResponse1.trim();
  sdiResponse2.trim();
  sdiResponse3.trim();
  rawSensorResponse = sdiResponse1+"+"+sdiResponse2+"+"+sdiResponse3;
  rawSensorResponse = replacePlusWithSpace(rawSensorResponse);
  rawSensorResponse = insertSpaceBeforeMinus(rawSensorResponse);

  writeSensorDataToStruct(SensorData);

  return true; // all conditions met
}

// Funktion zum Ersetzen von '+' durch Leerzeichen
String replacePlusWithSpace(const String& input) {
  String modifiedInput;
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    if (c == '+') {
      modifiedInput += ' ';
    } else {
      modifiedInput += c;
    }
  }
  return modifiedInput;
}

// Funktion zum Einfügen von Leerzeichen vor jedem '-'
String insertSpaceBeforeMinus(const String& input) {
  String modifiedInput;
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    if (c == '-') {
      // Füge ein Leerzeichen hinzu, falls das letzte Zeichen nicht bereits ein Leerzeichen ist
      if (i > 0 && modifiedInput[modifiedInput.length() - 1] != ' ') {
        modifiedInput += ' ';
      }
      modifiedInput += c;
    } else {
      modifiedInput += c;
    }
  }
  return modifiedInput;
}


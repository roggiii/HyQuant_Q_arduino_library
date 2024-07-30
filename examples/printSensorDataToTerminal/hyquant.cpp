/*
Copyright (c) <2024>, <AlpGeotech>
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree. 
*/


#include "hyquant.h"  //  Header file for this library

struct HyQuant_Data SensorData;
String rawSensorResponse;

float extractFilteredLevel()
{
  return getValue(rawSensorResponse, ' ', 1).toFloat();
}

float extractCurrentDistance()
{
  return getValue(rawSensorResponse, ' ', 8).toFloat();
}

float extractDischarge()
{
  return getValue(rawSensorResponse, ' ', 5).toFloat();
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

// prints Struct to Terminal
void printData()
{
  Serial.print("# sensor adress: "); Serial.println(SensorData.sensorAdress1);
  Serial.print("# filtered level: "); Serial.println(SensorData.filteredLevel,2);
  Serial.print("# filtered velocity: "); Serial.println(SensorData.filteredVelo,2);
  Serial.print("# SNR level: "); Serial.println(SensorData.SNRlevel);
  Serial.print("# SNR velocity: "); Serial.println(SensorData.SNRvelo);
  Serial.print("# discharge: "); Serial.println(SensorData.discharge,2);

  Serial.print("# sensor adress: "); Serial.println(SensorData.sensorAdress2);
  Serial.print("# current level: "); Serial.println(SensorData.currentLevel,2);
  Serial.print("# current distance: "); Serial.println(SensorData.currentDistance,2);
  Serial.print("# current velocity: "); Serial.println(SensorData.currentVelo,2);
  Serial.print("# quality level: "); Serial.println(SensorData.qualityLevel);
  Serial.print("# quality discharge: "); Serial.println(SensorData.qualitydischarge);

  Serial.print("# sensor adress: ");            Serial.println(SensorData.sensorAdress3);
  Serial.print("# temperature in enclosure: "); Serial.println(SensorData.temp,2);
  Serial.print("# tilt angle x direction: ");   Serial.println(SensorData.tiltX,2);
  Serial.print("# tilt angle y direction: ");   Serial.println(SensorData.tiltY,2);
  Serial.print("# sensor height above reference: "); Serial.println(SensorData.heightAboveRef,2);
}

bool isOnline(RAK_SDI12 mySDI12)
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
bool getParameters(RAK_SDI12 mySDI12){

  static String sdiResponse;
  static String sdiResponse1;
  static String sdiResponse2;
  static String sdiResponse3;

  // checking if sensor is running
  if(isOnline(mySDI12) == false)
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
      return 0;
    }
  }
  sdiResponse1.trim();
  sdiResponse2.trim();
  sdiResponse3.trim();
  rawSensorResponse = sdiResponse1+"+"+sdiResponse2+"+"+sdiResponse3;
  rawSensorResponse = replacePlusWithSpace(rawSensorResponse);
  rawSensorResponse = insertSpaceBeforeMinus(rawSensorResponse);

  extractFilteredLevel();
  extractDischarge();

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


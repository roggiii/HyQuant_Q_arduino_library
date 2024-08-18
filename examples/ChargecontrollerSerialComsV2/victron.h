/*
Copyright (c) <2024>, <AlpGeotech>
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree. 
*/

#ifndef VICTRON_H_
#define VICTRON_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

void RecvWithEndMarker(SoftwareSerial victronSerial);
void HandleNewData();
void ParseData();
void PrintEverySecond();
void PrintValues();


#endif // VICTRON_H_
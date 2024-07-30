/**
Copyright (c) <2024>, <AlpGeotech>
All rights reserved.
This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree. 
*/

#include "hyquant.h"


#define TX_PIN    WB_IO6   // The pin of the SDI-12 data bus.
#define RX_PIN    WB_IO5   // The pin of the SDI-12 data bus.
#define OE        WB_IO4   // Output enable pin, active low.

RAK_SDI12 mySDI12(RX_PIN,TX_PIN,OE);

  int alive_bit = 0;

void setup()
{

  // Initialize Serial for debug output.
  time_t timeout = millis();
	Serial.begin(115200);
  while (!Serial)
  {
    if ((millis() - timeout) < 5000)
    {
      delay(100);
    }
    else
    {
      break;
    }
  }

  Serial.println("Opening SDI-12 bus.");
  mySDI12.begin();
  delay(500);

  //mySDI12.forceListen(); did make problems when the sensor was starting up


}

void loop() 
{
  if (Serial.available()) 
  {
    char inByte1 = Serial.read();
    if (inByte1 == '\r' || inByte1 == '\n') 
    {
      if(getParameters(mySDI12))
      {
      }
      
    } 
  }
  if(getParameters(mySDI12)){
    Serial.println("-------- Sensor Data ------------");
    Serial.print("filtered sensor level: "); Serial.println(extractFilteredLevel());
    Serial.print("current distance: "); Serial.println(extractCurrentDistance());
    Serial.print("discharge: "); Serial.println(extractDischarge());
    Serial.print("alive bit: "); Serial.println(alive_bit);
  }

  alive_bit++;
}
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

// create SDI-12-bus-object
RAK_SDI12 mySDI12(RX_PIN,TX_PIN,OE);

// getParamers() writes sensor data to this struct
struct HyQuant_Data SensorData;

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


}

void loop() 
{

  if(getParameters(mySDI12, SensorData)){
    Serial.println("-------- Sensor Data ------------");
    Serial.print("filtered sensor level: "); Serial.println(SensorData.filteredLevel);
    Serial.print("current distance: "); Serial.println(SensorData.currentDistance);
    Serial.print("discharge: "); Serial.println(SensorData.discharge);

  }


}
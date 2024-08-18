/*
    Victron.Arduino-ESP8266
    A:Pim Rutgers
    E:pim@physee.eu

    Code to grab data from the VE.Direct-Protocol on Arduino / ESP8266.
    Tested on NodeMCU v1.0

    The fields of the serial commands are configured in "config.h"

*/

#include "victron.h"
#include <SoftwareSerial.h>

// Serial variables
//#define rxPin WB_IO3

#define rxPin 1
#define txPin 999                                    // TX Not used
//SoftwareSerial victronSerial(rxPin, txPin);         // RX, TX Using Software Serial so we can use the hardware serial to check the ouput via the USB serial provided by the NodeMCU.

void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(19200);
    SoftwareSerial victronSerial(1, 1);
    victronSerial.begin(19200);
}

void loop() {

    // Receive information on Serial from MPPT
    //RecvWithEndMarker(victronSerial);
    HandleNewData(victronSerial);

    // Just print the values every second,
    // Add your own code here to use the data. 
    // Make sure to not used delay(X)s of bigger than 50ms,
    // so make use of the same principle used in PrintEverySecond() 
    // or use some sort of Alarm/Timer Library
    PrintEverySecond();
}
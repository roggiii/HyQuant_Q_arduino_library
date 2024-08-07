

#include"Wire.h"   //needed by RAK4631 if Serial is used

struct uart_data_structure {
  uint32_t timestamp;
  char alarm;
  float filteredVelo;
  float discharge;
  float filteredLevel;
};

struct uart_data_structure uart_data = {1499070300000, 'a' , 50.1, 50.2, 50.3};
int data_size = sizeof(struct uart_data_structure);

void setup() {
  Serial.begin(9600); // initialise UART for serial communication over usb for debug purpuses
  Serial1.begin(9600); // initialise UART pins that go to the extension board
}

void loop() 
{

send(&uart_data);
delay(1000);

}

// missing: delimitor to signalize begining of the string

void send (const uart_data_structure* table)
{
  Serial.write((uint8_t*)table, data_size);  // 2 bytes.
}

bool receive(uart_data_structure* table)
{
  return (Serial.readBytes((uint8_t*)table, data_size) == data_size);
}

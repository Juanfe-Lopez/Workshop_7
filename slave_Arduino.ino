#include <Wire.h>

float valorSensor = 0.0;

void setup() {
  Wire.begin(8); // Initialize the slave with address 8
  Wire.onRequest(sendData); // Set up the function to handle data requests
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  
  int reading = analogRead(A0);

  // Convert the reading into voltage:
  float voltage = reading * (5000 / 1024.0);

  // Convert the voltage into the temperature in degree Celsius:
  valorSensor= voltage / 10;

}

void sendData() {
  // This function is called when the master requests data
  Wire.write((byte*)&valorSensor, sizeof(valorSensor)); // Send the float value
}

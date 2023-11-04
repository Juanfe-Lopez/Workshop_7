#include <Wire.h>
#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "UNICABLE_ORTEGA_LOPEZ";   // your network SSID (name) 
const char* password = "39034937";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 2;
const char * myWriteAPIKey = "17WJ3X28O6CQIMXM";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;


float valorRecibido = 0.0;
int slaveAddress = 8;
const int ledPin = 18; // GPIO pin for the LED

void setup() {
    Wire.begin();
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT); // Set the LED pin as an output
    WiFi.mode(WIFI_STA);   
  
    ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    Wire.requestFrom(slaveAddress, sizeof(valorRecibido));
    if (Wire.available()) {
        Wire.readBytes((char*)&valorRecibido, sizeof(valorRecibido));
        Serial.print("Valor recibido: ");
        Serial.println(valorRecibido);

        if (valorRecibido > 30) {
            digitalWrite(ledPin, HIGH); // Turn on the LED
        } else {
            digitalWrite(ledPin, LOW); // Turn off the LED
        }
    }
    
    
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeField(myChannelNumber, 1, valorRecibido, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
    
}

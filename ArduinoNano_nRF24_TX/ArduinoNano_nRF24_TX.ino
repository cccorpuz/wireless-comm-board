// see nrf24.github.io/RF24/md_docs_attiny.html for pin references

// IEEE Project Fall 2021
// see nrf24.github.io/RF24/md_docs_attiny.html for pin references

// IEEE Project Fall 2021
// Project Engineer: Crispin Corpuz
// Made for Arduino Nano

// This program is for transmission

#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include "RF24.h"

#define radioReceiver     0
#define radioTransmitter  1
#define CE                7
#define CSN               8

RF24 radio(CE, CSN); //CE, CSN

// One way paths to communicate between a pair of radios, since they are half-duplex
const byte networkSegment[6] = "00001";

uint16_t payload = 0;

void setup() {
  // put your setup code here, to run once:

  // Run I2C sensor, etc. setup here
  Serial.begin(9600);
  while(!Serial) {}

  // Setting up nRF24L01+
  while (!radio.begin()) {}
  radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is the default; this prevents power supply issues
  radio.setPayloadSize(sizeof(payload)); // 2 bytes size
  radio.openWritingPipe(networkSegment); // defining segment to transmit on
  radio.stopListening(); // put radio into TX mode
  
  Serial.print("radio found and transmitting!\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long start = micros();
  bool transmissionSuccess = radio.write(&payload, sizeof(uint16_t));
  unsigned long endtime = micros();
  payload++;
  if (transmissionSuccess) 
  {
    Serial.print(F("Transmission successful! "));          // payload was delivered
    Serial.print(F("Time to transmit = "));
    Serial.print(endtime - start);                 // print the timer result
    Serial.print(F(" us. Sent: "));
    Serial.println(payload);                               // print payload sent
  }
  else 
  {
    Serial.println("Failed to send!");
  }

  delay(2000);
}

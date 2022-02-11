// see nrf24.github.io/RF24/md_docs_attiny.html for pin references

// IEEE Project Fall 2021
// Project Engineer: Crispin Corpuz
// Made for Arduino Nano

// This program is for receiving

#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include "RF24.h"

#define radioReceiver     0
#define radioTransmitter  1
#define CE                7
#define CSN               8

RF24 radio(CE, CSN); //CE, CSN pins respectively
LiquidCrystal_I2C lcd(0x27, 16, 2);

// One way paths to communicate between a pair of radios, since they are half-duplex
const byte networkSegment[6] = "00001";

uint16_t payload = 0;
uint16_t prevPayload = 0;
uint16_t packetsReceived = 0;
uint16_t packetsDropped = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  while(!Serial) {}
  if (!radio.begin()) {
    Serial.println("radio hardware is not responding!!");
    while (1) {} // hold in infinite loop
  }
  // Run I2C LCD, sensor, etc. here
  lcd.begin(16, 2);         // Initialize the I2C LCD
  lcd.home();
  lcd.backlight();
  //  lcd.cursor();
  //  lcd.blink();
  lcd.print("Initializing...");

  // Setting up nRF24L01+
  radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is the default; this prevents power supply issues
  radio.setPayloadSize(sizeof(payload)); // 2 bytes size
  radio.openReadingPipe(0, networkSegment); // defining segment to receive on
  radio.startListening(); // put radio into RX mode

  // ready to receive
  lcd.clear();
  lcd.print("RTR, waiting...");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    lcd.clear();

    // receive payload from transmitter
    uint8_t payloadSizeInBytes = radio.getPayloadSize();
    radio.read(&payload, payloadSizeInBytes);
    Serial.println(payload);
    lcd.home();
    if (payload - prevPayload == 1) packetsReceived++;
    else packetsDropped++;
    prevPayload = payload;
    lcd.print("Received: ");
    lcd.print(packetsReceived);
    lcd.setCursor(0,1);
    lcd.print("Dropped: ");
    lcd.print(packetsDropped);
    lcd.home();
  }
}

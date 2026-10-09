// Acadia University - COMP 5163
//
// Digital Input/Output Lab - Test Program
//
#include <Arduino.h>
void setup() {
  // configure the USB serial monitor
  Serial.begin(115200);
  // configure pin D5 as digital input - this is the pushbutton
  pinMode(D5, INPUT_PULLUP);
  // configure pin D4 as a digital output - this is the LED
  pinMode(D4, OUTPUT);
}
void loop() {
  int iButton;
  // read the push button input...
  iButton = digitalRead(D5);
  // ... and write it to the LED output
  digitalWrite(D4, iButton);
}
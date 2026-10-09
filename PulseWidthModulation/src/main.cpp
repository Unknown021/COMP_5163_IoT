// Acadia University - COMP 5163
//
// Pulse Width Modulation - Test Program
//
#include <Arduino.h>
void setup() {
  // configure the USB serial monitor
  Serial.begin(115200);
  // configure pin D4 as a digital output - this is the LED
  pinMode(D4, OUTPUT);
  // set default PWM range
  analogWriteRange(1023);
}
void loop() {
  int i;
  for ( i=0; i<1024; ++i) {
    // set the PWM pulse width
    analogWrite(D4, i);
    // wait 1ms to add DRAMA!
    delay(1);
  }
}

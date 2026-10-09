// Acadia University - COMP 5163
//
// Light Dimmer
// Statement of Authorship - 2026-10-09
// Author: Michael Berryman
// Version: 1.0
//
#include <Arduino.h>
bool _bLightOn = false, _bLastButtonValWasLow = false;
void setup() {
  Serial.begin(115200);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D4, OUTPUT);
  pinMode(A0, INPUT);
  analogWriteRange(1023);
}
void loop() {
  int iVariableResistorVal = analogRead(A0), iButtonVal = digitalRead(D5);
  bool bButtonPressedAndLetGo = (iButtonVal == HIGH && _bLastButtonValWasLow);
  _bLastButtonValWasLow = (iButtonVal == LOW);
  
  if (bButtonPressedAndLetGo) {
      _bLightOn = !_bLightOn;
  }
  if (_bLightOn) {
      analogWrite(D4, iVariableResistorVal);
  } else {
      analogWrite(D4, 1023);
  }
}

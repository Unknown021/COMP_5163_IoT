/*
  Statement of Authorship - 2026-10-08
  I, Michael Berryman, declare that this source code file is my own work.
  Other than the source code provided by the instructor, I have not used any other person's work.
  I have not made my work available to anyone else.
*/
#include <Arduino.h>

float INPUT_VOLTAGE = 3.3;
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
}

void loop() {
  int iVal;
  float fVolts, fTemp;
  char sJudgement[9] = "";
  iVal = analogRead(A0);
  // 1023 (1024 - 1) is the maximum reading for 0 resistance
  fVolts = (iVal * INPUT_VOLTAGE / 1023.0);
  // Pretend the voltage represents a reading from a temperature sensor that ranges from 0 to 50 celcius
  fTemp = (fVolts * 50.0 / INPUT_VOLTAGE);
  if (fTemp < 10.0) {
    strcpy(sJudgement, "Cold!");
  } else if (fTemp >= 10 && fTemp <= 15.0) {
    strcpy(sJudgement, "Cool");
  } else if (fTemp > 15 && fTemp <= 25.0) {
    strcpy(sJudgement, "Perfect");
  } else if (fTemp > 25 && fTemp <= 30.0) {
    strcpy(sJudgement, "Warm");
  } else if (fTemp > 30 && fTemp <= 35.0) {
    strcpy(sJudgement, "Hot");
  } else {
    strcpy(sJudgement, "Too Hot!");
  }

  Serial.printf("Digitized Value of %d is equivalent to an Analog Voltage of %.2f volts, and with a range of 0-50°C translates to %.2f°C (%s)\n", iVal, fVolts, fTemp, sJudgement);
  delay(2000);
}
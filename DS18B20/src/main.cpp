// Acadia University - COMP 5163
//
// DS18B20
// Statement of Authorship - 2026-10-21
// Author: Michael Berryman
// Version: 1.0
//
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int ONE_WIRE_BUS = D3;
OneWire _oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&_oneWire);
bool _bSensorPresent = false;

void setup() {
  Serial.begin(115200);
  // newline for readability
  Serial.println();
  DS18B20.begin();
  DeviceAddress address;
  if (DS18B20.getAddress(address, 0)) {
    char sAddress[17];
    for (uint8_t i = 0; i < sizeof(DeviceAddress); i++) {
      sprintf(&sAddress[i * 2], "%02X", address[i]);
    }
    Serial.printf("Found DS18B20 sensor with address: %s\n", sAddress);
    _bSensorPresent = true;
  }
  else
  {
    Serial.println("No DS18B20 temperature sensors are installed!");
  }
}

void loop() {
  if (!_bSensorPresent) {
    return;
  }
  float fTemp;
  char sJudgement[9] = "";
  DS18B20.requestTemperatures();
  fTemp = DS18B20.getTempCByIndex(0);
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
  Serial.printf("Current temperature is: %.2f°C or %s\n", fTemp, sJudgement);
  delay(5000);
}

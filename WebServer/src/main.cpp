// Acadia University - COMP 5163
//
// Web Server with DS18B20 Temperature Sensor
// Statement of Authorship - 2026-10-23
// Author: Michael Berryman
// Version: 1.0
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* SSID = "XXXXXXXXXXXXX";
const char* PASSWORD = "XXXXXXXXXXXXX";
WiFiServer server(80);
const int ONE_WIRE_BUS = D3;
OneWire _oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&_oneWire);
bool _bSensorPresent = false;
char _sAddress[17] = "\0";
char _localIp[16] = "\0";
enum REQUEST_TYPE { PAGE, API, UNKNOWN };

void setup()
{
  Serial.begin(115200);
  Serial.println("\nWeb Server with DS18B20 Temperature Sensor");
  // Setup the temperature sensor
  Serial.printf("\nDetecting temperature sensor on pin %d\n", ONE_WIRE_BUS);
  DS18B20.begin();
  DeviceAddress address;
  if (DS18B20.getAddress(address, 0)) {
    for (uint8_t i = 0; i < sizeof(DeviceAddress); i++) {
      sprintf(&_sAddress[i * 2], "%02X", address[i]);
    }
    Serial.printf("Found DS18B20 sensor with address: %s\n", _sAddress);
    _bSensorPresent = true;
  }
  else
  {
    Serial.println("No DS18B20 temperature sensors are installed!");
  }
  // Setup the web server
  Serial.printf("\nConnecting to %s\n", SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!\n\nStarting web server...\n");
  server.begin();
  strcpy(_localIp, WiFi.localIP().toString().c_str());
  Serial.printf("\n\nWeb server started, open %s in a web browser\n", _localIp);
}

void loop()
{
  REQUEST_TYPE requestType = UNKNOWN;
  WiFiClient client = server.accept();
  if (client)
  {
    Serial.println("\n>> Client connected");
    Serial.println(">> Client request:");
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        if (line.startsWith("GET /api/ds18b20"))
        {
          requestType = API;
        }
        else if (line.startsWith("GET /"))
        {
          requestType = PAGE;
        }
        if (line.length() == 1 && line[0] == '\n')
        {
          // we've reached the end of the request
          if (requestType == UNKNOWN)
          {
            Serial.println(" >> Unknown request type, sending 404 response");
            client.println("HTTP/1.1 404 Not Found");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<html><body><h1>404 Not Found</h1></body></html>");
            break;
          }
          if (requestType == PAGE)
          {
            long lngMsSinceStart = millis();
            float fTemp = 0.0;
            if (_bSensorPresent) {
              DS18B20.requestTemperatures();
              fTemp = DS18B20.getTempCByIndex(0);
            }
            const char* htmlPart1 = ""
            "<!DOCTYPE html>\n"
            "  <html>\n"
            "    <head>\n"
            "      <title>Web Server with DS18B20 Temperature Sensor</title>\n"
            "      <style tyle=\"text/css\">\n"
            "        body {\n"
            "          font-family: Arial, sans-serif;\n"
            "          background-color: #f2f2f2;\n"
            "          margin: 5px;\n"
            "          text-align: center;\n"
            "        }\n"
            "        h1 {\n"
            "          color: #333;\n"
            "          text-decoration: underline;\n"
            "        }\n"
            "        p {\n"
            "          font-size: 2em;\n"
            "          color: #666;\n"
            "        }\n"
            "      </style>\n"
            "      <script type=\"text/javascript\">\n"
            "        var ms = %ld;"
            "        function calcTimeString() {\n"
            "          var hours = Math.floor(ms / 3600000);\n"
            "          var minutes = Math.floor((ms %% 3600000) / 60000);\n"
            "          var seconds = Math.floor((ms %% 60000) / 1000);\n"
            "          return `${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;\n"
            "        }\n"
            "        function updateTimeString() {\n"
            "          document.getElementById('time').innerHTML = calcTimeString();\n"
            "        }\n"
            "        function updateTime() {\n"
            "          ms += 1000;\n"
            "          updateTimeString();\n"
            "        }\n"
            "        function updateTemperatureInfo() {\n"
            "          var xhr = new XMLHttpRequest();\n"
            "          xhr.onreadystatechange = function() {\n"
            "            if (xhr.readyState === 4 && xhr.status === 200) {\n"
            "              var response = JSON.parse(xhr.responseText);\n"
            "              document.getElementById('temperature').textContent = response.temperature.toFixed(2);\n"
            "            }\n"
            "          };\n"
            "          xhr.open('GET', '/api/ds18b20', true);\n"
            "          xhr.send();\n"
            "        }\n"
            "        function onLoaded() {\n"
            "          updateTimeString();\n"
            "          setInterval(updateTime, 1000);\n"
            "          setInterval(updateTemperatureInfo, 5000);\n"
            "        };\n"
            "        document.addEventListener('DOMContentLoaded', onLoaded);\n"
            "      </script>\n"
            "    </head>\n";
            const char* htmlPart2 = ""
            "    <body>\n"
            "      <h1>Student Information</h1>\n"
            "      <p><strong>Name:</strong> Michael Berryman</p>\n"
            "      <p><strong>Student ID:</strong> 0317491b</p>\n"
            "      <hr />\n"
            "      <h1>Arduino Information</h1>\n"
            "      <p>Arduino has been running for <span id=\"time\"></span></p>\n"
            "      <hr />\n"
            "      <h1>Web Server Information</h1>\n"
            "      <p>Web server is running on IP address: %s</p>\n"
            "      <hr />\n"
            "      <h1>DS18B20 Temperature Sensor Information</h1>\n"
            "      <p>Temperature sensor is %s</p>\n"
            "      <p>Current temperature is: <span id=\"temperature\">%.2f</span>&deg;C</p>\n"
            "      <p>Sensor address is: %s</p>\n"
            "      <br/></br><br/>\n"
            "      <button style=\"background-color: gray; color: white; font-size: 2em; padding: 10px 20px; cursor: pointer;\" onclick=\"location.reload();\">Refresh</button>\n"
            "    </body>\n"
            "  </html>\n";
            char* htmlPart1Raw = NULL;
            char* htmlPart2Raw = NULL;
            asprintf(&htmlPart1Raw, htmlPart1, lngMsSinceStart);
            asprintf(&htmlPart2Raw, htmlPart2, _localIp, _bSensorPresent ? "present" : "not present", fTemp, _sAddress);
            client.print(htmlPart1Raw);
            client.print(htmlPart2Raw);
            free(htmlPart1Raw);
            free(htmlPart2Raw);
            Serial.printf(" >> Response sent to client\n");
            Serial.print(htmlPart1Raw);
            Serial.print(htmlPart2Raw);
            Serial.println();
            Serial.printf(" >> Variable values\n");
            Serial.printf(" >> lngMsSinceStart: %ld\n", lngMsSinceStart);
            Serial.printf(" >> _localIp: %s\n", _localIp);
            Serial.printf(" >> fTemp: %.2f\n", fTemp);
            Serial.printf(" >> _sAddress: %s\n", _sAddress);
            break;
          }
          if (requestType == API)
          {
            float fTemp = 0.0;
            if (_bSensorPresent) {
              DS18B20.requestTemperatures();
              fTemp = DS18B20.getTempCByIndex(0);
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println();
            client.printf("{\"temperature\": %.2f}", fTemp);
            Serial.printf(" >> Response sent to client\n");
            Serial.printf(" >> Variable values\n");
            Serial.printf(" >> fTemp: %.2f\n", fTemp);
            break;
          }
        }
      }
    }
    while (client.available()) {
      client.read();
    }
    client.stop();
    Serial.println(">> Client disconnected");
  }
}
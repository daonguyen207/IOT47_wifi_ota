#include "iot47_wifi_ota.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#else
#include <WiFi.h>
#include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

const char* ssid = "qstech";
const char* password = "0123456789";

AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  //iot47_ota_set_debug(false);
  iot47_wifi_ota_begin(&server);
  server.begin();
}

void loop() 
{
  iot47_wifi_ota_loop();
}

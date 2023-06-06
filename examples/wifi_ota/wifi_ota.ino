#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "iot47_wifi_ota.h"

// Replace with your network credentials
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

  iot47_wifi_ota_begin(&server);
  server.begin();
}

void loop() 
{
  iot47_wifi_ota_loop();
}

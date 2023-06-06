#ifndef _IOT47_WIFI_OTA_H_
#define _IOT47_WIFI_OTA_H_

#include "ota_index_page.h"
#include <ESPAsyncWebServer.h>
#include <Update.h>

static char flag = 0;
int UpdateRun(bool _SIZE_UNKNOWN)
{
  if (Update.end(_SIZE_UNKNOWN))
  {
      Serial.println("OTA finished!");
      if (Update.isFinished())
      {
          Serial.println("Restart device!");
          delay(2000);
          ESP.restart();
      }
      else
      {
          Serial.println("OTA not fiished");
      }
  }
  else
  {
      Serial.println("Error occured #: " + String(Update.getError()));
  }
}
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if(!index)
  {
    Serial.println("UploadStart");
    Update.begin(UPDATE_SIZE_UNKNOWN);
  }
  Update.write(data,len);
  if(final)
  {
    Serial.printf("\r\nUploadEnd: %s, %u B\n", filename.c_str(), index+len);
    flag = 1;
  }
}
void iot47_wifi_ota_begin(AsyncWebServer *s)
{
  s->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {request->send(200);},handleUpload);
  s->on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {request->send_P(200, "text/html", uploadPage);});
}
void iot47_wifi_ota_loop()
{
  if(flag==1)
  {
    flag=0;
    delay(1000);
    UpdateRun(true);
  }
}

#endif
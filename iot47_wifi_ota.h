#ifndef _IOT47_WIFI_OTA_H_
#define _IOT47_WIFI_OTA_H_

#include "ota_index_page.h"
#include <ESPAsyncWebServer.h>
#include <Update.h>

static char flag = 0;
int UpdateRun()
{
    Serial.println("OTA finished!");
    Serial.printf("Hash in chip: %s\n", Update.md5String().c_str());
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
    if(request->hasHeader("Size"))
    {
      AsyncWebHeader* h = request->getHeader("Size");
      uint32_t totol_size = h->value().toInt();
      if(totol_size == (index+len))
      {
        if(request->hasHeader("Hash"))
        {
          AsyncWebHeader* h = request->getHeader("Hash");
          Update.setMD5((const char *)h->value().c_str());
          Serial.printf("Hash: %s\n", h->value().c_str());
          if (Update.end(true))
          {
            Serial.printf("\r\nUploadEnd: %s, %u B\n", filename.c_str(), totol_size);
            flag = 1;
          }
          else
          {
            Serial.println(F("MD5 hash fail !"));
            Serial.println("Error occured #: " + String(Update.getError()));
            request->send(400,"MD5 error");
          }       
        }
        else
        {
          Serial.println(F("No Hash, update fail"));
          request->send(400);
        }
      }
      else
      {
        Serial.println(F("Size firmware error"));
        request->send(400);
      }
    }
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
    UpdateRun();
  }
}

#endif

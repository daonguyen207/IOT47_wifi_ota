#ifndef _IOT47_WIFI_OTA_H_
#define _IOT47_WIFI_OTA_H_

#include "ota_index_page.h"
#include <ESPAsyncWebServer.h>
#if defined(ESP8266)
#include <Updater.h>
#else
#include <Update.h>
#endif

static char flag = 0;
static uint32_t totol_size;
static uint32_t curren_download;
static char show_debug=true;

typedef void (*wifiota_callback_t)(uint32_t curen, uint32_t totol);
wifiota_callback_t wbegin_callback;
wifiota_callback_t wproces_callback;
wifiota_callback_t wend_callback;

void iot47_wifi_ota_set_begin_callback(wifiota_callback_t c)
{
  wbegin_callback = c;
}
void iot47_wifi_ota_set_proces_callback(wifiota_callback_t c)
{
  wproces_callback = c;
}
void iot47_wifi_ota_set_end_callback(wifiota_callback_t c)
{
  wend_callback = c;
}


void iot47_ota_set_debug(bool mode)
{
   show_debug = mode;
}

int UpdateRun()
{
    if(show_debug)Serial.println("OTA finished!");
    if(show_debug)Serial.printf("Hash in chip: %s\n", Update.md5String().c_str());
    if (Update.isFinished())
    {
        if(show_debug)Serial.println("Restart device!");
        delay(2000);
        ESP.restart();
    }
    else
    {
        if(show_debug)Serial.println("OTA not fiished");
    }
}
static void handleUploadOTA(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if(!index)
  {
    if(request->hasHeader("Size"))
    {
      const AsyncWebHeader* h = request->getHeader("Size");
      totol_size = h->value().toInt();
      #if defined(ESP8266)
      Update.runAsync(true);
      #endif
      Update.begin(totol_size);
      if(show_debug)Serial.println("UploadStart: " + String(totol_size));
      curren_download=0;
      if(wbegin_callback)wbegin_callback(curren_download,totol_size);
    }
    else
    {
      if(show_debug)Serial.println(F("No Size, update fail"));
      request->send(400);
    }
  }
  Update.write(data,len);
  curren_download+=len;
  if(wproces_callback)wproces_callback(curren_download,totol_size);
  if(final)
  {
    if(totol_size == (index+len))
    {
      if(request->hasHeader("Hash"))
      {
        const AsyncWebHeader* h = request->getHeader("Hash");
        Update.setMD5((const char *)h->value().c_str());
        if(show_debug)Serial.printf("Hash: %s\n", h->value().c_str());
        if (Update.end(true))
        {
          if(show_debug)Serial.printf("\r\nUploadEnd: %s, %u B\n", filename.c_str(), totol_size);
          if(wend_callback)wend_callback(curren_download,totol_size);
          flag = 1;
        }
        else
        {
          if(show_debug)Serial.println(F("MD5 hash fail !"));
          if(show_debug)Serial.println("Error occured #: " + String(Update.getError()));
          request->send(400,"MD5 error");
        }       
      }
      else
      {
        if(show_debug)Serial.println(F("No Hash, update fail"));
        request->send(400);
      }
    }
    else
    {
      if(show_debug)Serial.println(F("Size firmware error"));
      request->send(400);
    }
  }
}
void iot47_wifi_ota_begin(AsyncWebServer *s)
{
  s->on("/uploadfw", HTTP_POST, [](AsyncWebServerRequest *request) {request->send(200);},handleUploadOTA);
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


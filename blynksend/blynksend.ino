#define BLYNK_TEMPLATE_ID "TMPL2aOEB9ZkA"
#define BLYNK_TEMPLATE_NAME "Green House"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

char ssid[] = "INFINITUM0D20";
char pass[] = "TejpN9RBjK";


//#define USE_ESP32_DEV_MODULE

#include "BlynkEdgent.h"
#include <ArduinoJson.h>

String recv_str_jsondata;

StaticJsonDocument<256> doc_recv;

#define RXD2 16
#define TXD2 17

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);        
  delay(100);

  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();

  if (Serial2.available())
  {

    recv_str_jsondata = Serial2.readStringUntil('\n');
    Serial.println(recv_str_jsondata);
    DeserializationError error = deserializeJson(doc_recv, recv_str_jsondata);

    if (!error) {
      float h1 =  doc_recv["v1"];
      float h2 =  doc_recv["v2"];
      if(h1 == 0){h1 = 4095;}
      if(h2 == 0){h2 = 4095;}                                   
      float hum1 = map(h1, 2048, 4095, 100, 0);                       
      float hum2 = map(h2, 2048, 4095, 100, 0);                       

      if (hum1 > 0) {
        Blynk.virtualWrite(V1, hum1);
        Serial.println("sensor 1 = " + String(hum1));
      }
      if (hum2 > 0) {
        Blynk.virtualWrite(V2, hum2);
        Serial.println("sensor 2 = " + String(hum2));
      }

    }

    else {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    recv_str_jsondata = "";
  }
}


#define BLYNK_TEMPLATE_ID "TMPLkMNvOYYR"
#define BLYNK_DEVICE_NAME "ESPNOW"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial

#include "BlynkEdgent.h"
#include <ArduinoJson.h>

String recv_str_jsondata;

StaticJsonDocument<256> doc_recv;

#define RXD2 16
#define TXD2 17


void setup()
{
  BlynkEdgent.begin();

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);        //UART

}

void loop() {

  BlynkEdgent.run();
  
  // Recibir datos del micro que coordina
  if (Serial2.available())
  {

    recv_str_jsondata = Serial2.readStringUntil('\n');
    Serial.println(recv_str_jsondata);
    DeserializationError error = deserializeJson(doc_recv, recv_str_jsondata);

    if (!error) {                                           
      float hum1 = doc_recv["v1"];                       
      float hum2 = doc_recv["v2"];                       

      if (temp > 0) {
        Blynk.virtualWrite(V1, hum1);                     //sensor 1
        Serial.print("sensor 1 ="); Serial.println(hum1);
      }
      if (hum > 0) {
        Blynk.virtualWrite(V2, hum2);                     // sensor 2, se aplica la condición del 0 por que ambos micros envían el dato
        Serial.print("sensor 2 ="); Serial.println(hum2);
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

#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h> 

StaticJsonDocument<256> doc_from_espnow; 

String recv_jsondata;    //string JSON

// UART
#define RXD2 16
#define TXD2 17

/*Funcion para recibir datos, es por defecto*/
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{

  char* buff = (char*) incomingData;
  recv_jsondata = String(buff);
  Serial.print("Recieved from ESPNOW: "); Serial.println(recv_jsondata);
  DeserializationError error = deserializeJson(doc_from_espnow, recv_jsondata);
  if (!error) {
    Serial.print("Serilising to Serial2: ");
    Serial.println(recv_jsondata);
    serializeJson(doc_from_espnow, Serial2);  // datos que se envian por UART
  }

  else
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

}

void setup() {

  //led integrado
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(2000);
  digitalWrite(2, LOW);
  delay(2000);

  // Inicializar UART
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);//protocolo de 8 bits, 1 stop bit

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //callback para recibir datos
  esp_now_register_recv_cb(OnDataRecv);

}

void loop()
{

}

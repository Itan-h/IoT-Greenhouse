#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {0xA0, 0xB7, 0x65, 0x4C, 0x0A, 0xEC}; /*dirección MAC de quien recibe los datos*/

#include <ArduinoJson.h>//JSON

String send_jsondata;
StaticJsonDocument<256> doc_to_espnow;
const int readHumedity = 4;
//-----------------------------------------------------------------

//callback para verificar que se enviaron los datos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  

  //inicializar esp now
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent); //se hace el callback           
  
  esp_now_peer_info_t peerInfo = {}; //pendiente de revisión = {} parametros peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  /*copia los 6 bytes de la dirección mac a la dirección peer*/
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // agregar peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  //PENDIENTE                                                
  float hum1 = 0;
  float hum2 = analogRead(readHumedity);
  doc_to_espnow["v1"] = hum1;                 //para est caso el sensor 2 es el que mide      
  doc_to_espnow["v2"] = hum2;                       
  serializeJson(doc_to_espnow, send_jsondata);
  esp_now_send(broadcastAddress, (uint8_t *) send_jsondata.c_str(), send_jsondata.length());
                                                    
  Serial.println(send_jsondata); 
  send_jsondata = "";
  delay(2000);
}

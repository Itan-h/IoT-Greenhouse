#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {00xA0, 0xB7, 0x65, 0x4C, 0x0A, 0xEC}; /*dirección MAC de quien recibe los datos*/

#include <ArduinoJson.h>//JSON, solo por el momento
String recv_jsondata;
String send_jsondata;
StaticJsonDocument<256> doc_to_espnow;
StaticJsonDocument<256> doc_from_espnow;  
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
  
  esp_now_peer_info_t peerInfo; //pendiente de revisión = {} parametros peer
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
  //PENDIENTE                                                // Sending Tempreature Every 4 Seconds
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  doc_to_espnow["v1"] = temp;                       // Creating JSON data. Here { v1 : 28.55 }
  doc_to_espnow["v2"] = hum;                       // Creating JSON data. Here { v2 : 34.35 }
  serializeJson(doc_to_espnow, send_jsondata);
  esp_now_send(broadcastAddress, (uint8_t *) send_jsondata.c_str(), send_jsondata.length());
                                                    // Sending it to Coordinater ESP
  Serial.println(send_jsondata); 
  send_jsondata = "";
  delay(4000);
}

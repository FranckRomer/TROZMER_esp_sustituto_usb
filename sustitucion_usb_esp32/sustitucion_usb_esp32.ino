#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <iostream>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

// RED A INTERNET
const char* ssid = "RED ACCESA";
const char* password = "037E32E7";

//const char* server = "http://192.168.1.121:3456/api/info";
const char* server = "http://192.168.1.113:3456/esp32";


/*
 * ********************************************************************
                 Setup
 * ********************************************************************
*/
void setup() {
  // Setup basico
  Serial.begin(115200);
  delay(50);
  Serial2.begin(115200);
  
  Serial.println(".");
  Serial.println(".");
  Serial.println(".");
  setupWiFi();

}


/*
 * ********************************************************************
              Programa principal
 * ********************************************************************
*/

void loop() {
  //Serial.println(".");
  if (Serial2.available()){
    //int rawData = Serial2.read();
    //Print value in HEX
    String data = Serial2.readStringUntil('\n');
    //DEBUG(data);
    Serial.println(data);
    ///////////////////////////////////////////////
//    String msm_serial2 = "";
//    msm_serial2 = String(Serial2.read());
//    //msm_serial2 = String((char *)msm_serial2);
//    Serial.println(Serial2.read());
//    
    char buffer[256];
    //dataSnd_string = String(json);
    StaticJsonDocument<300> doc;
    
    doc["dispositivo"]       = "MUX";
    doc["data"]              = data;


    size_t n = serializeJson(doc, buffer);
    //msm_res_api = buffer;
    POST_JSON(String(buffer));
  }  
  ///////////////////////////////////////////////

}

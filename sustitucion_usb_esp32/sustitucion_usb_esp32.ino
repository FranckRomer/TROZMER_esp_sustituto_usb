#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <iostream>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

// RED A INTERNET
const char* ssid = "RED ACCESA";
const char* password = "037E32E7";

const char* server = "http://192.168.1.121:3456/tarj";
//const char* server = "http://192.168.1.113:3456/esp32";

#define RXD1 4
#define TXD1 2

#define RXD2 16
#define TXD2 17


/*
 * ********************************************************************
                 Setup
 * ********************************************************************
*/
void setup() {
  // Setup basico
  Serial.begin(115200); // El TXD Y RXD del modulo son los de la programacion (debug)
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);// serial1 pines 4 y 2
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);// serial2 pines 16 y 17 
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
  if (Serial1.available()) {
     //Serial.print(char(Serial1.read()));
     String data = Serial1.readStringUntil('\n');
     //String data = char(Serial1.read())
     Serial.println(data);
     char buffer[256];
     StaticJsonDocument<300> doc;
     doc["oxi"]              = data;
     size_t n = serializeJson(doc, buffer);
     POST_JSON(String(buffer));
   }
  if (Serial2.available()){
    //int rawData = Serial2.read();
    //Print value in HEX
    String data = Serial2.readStringUntil('\n');
    //DEBUG(data);
    Serial.println(data);
    ///////////////////////////////////////////////

    char buffer[256];
    //dataSnd_string = String(json);
    StaticJsonDocument<300> doc;
    
    //doc["dispositivo"]       = "MUX";
    doc["tarj"]              = data;


    size_t n = serializeJson(doc, buffer);
    //msm_res_api = buffer;
    POST_JSON(String(buffer));
  }  
  ///////////////////////////////////////////////

}

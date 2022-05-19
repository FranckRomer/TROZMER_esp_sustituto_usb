/*
 * ********************************************************************
              POST_JSON
 * ********************************************************************
*/
void POST_JSON(String payload ){
  HTTPClient http;
  //http.begin("http://192.168.1.113:3001/api/v1/esp32/");
  http.begin(String(server));
  Serial.println("API: " + String(server));
  http.addHeader("Content-Type", "application/json");   
    
  Serial.println("----------------------------");
  Serial.println(payload);
  Serial.println("----------------------------");
  
    int httpResponseCode = http.POST(payload);

  if(httpResponseCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpResponseCode);
      if(httpResponseCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println("Servidor Res:" + String(payload));
          //boolean_SERVER = String(payload);
      }
  } else {
      //boolean_SERVER = false;
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      
  }
  
  http.end();
  //delay(200);
}

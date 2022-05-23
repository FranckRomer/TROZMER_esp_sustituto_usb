#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);    // d7 = rx = 13, d8 = tx = 15

void setup()  
{
  Serial.begin(115200);
  
  Serial.println("Connect! - Conexion ");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);  
  mySerial.print("Connect! - Conexion SOFTWARESERIAL");
}

void loop() // run over and over
{
  delay(10000);
  mySerial.write("15.14455");
  Serial.println("Se mando: 15.14455");
  delay(1000);
//  if (mySerial.available())
//    Serial.write(mySerial.read());
//  if (Serial.available())
//    mySerial.write(Serial.read());
}

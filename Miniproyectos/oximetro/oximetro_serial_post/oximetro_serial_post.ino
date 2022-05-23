#include <Wire.h>
#include "MAX30105.h"
#include <math.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);

MAX30105 particleSensor;
//UltraSonido/////
const int pinTrigger= 2; //Variable que contiene el número del pin al cual conectamos la señal "trigger"
const int pinEcho = 3;   //Variable que contiene el número del pin al cual conectamos la señal "echo"
const int pinGel = 10;   //Variable que contiene el número del pin al cual conectamos la salida "Gel"
const int dis_detec =9;
int flag_gel,ngel;
//////////////////
const int media_lec=20;
int beep_lec;
const int PIN_beep = 9;

float picmax=0;
float contadorrms=0;
int banderainicio=0;
int band=0;
float tiempopulsos=0;
float heartrate=0;
float heartrateant=0;
float iniciotiempo=0;
float inicioprograma=0;

float Signal=0.0;
float alpha=0.5;
float S=0.0;
long w=0;
long wprev=0;
long y=0;
float rms=0;
float R=7.18;
float media_R;
int myint;
long puls;
float saturacion=0;
/////
float tiempo=-49.5;
int contadorpulsos=0;
long irw=0;
long irwprev=0;
long iry=0;
float irSignal=0.0;
float irS=0.0;
float irms=0;

long oxi=0;

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[50]; //infrared LED sensor data
uint16_t redBuffer[50];  //red LED sensor data
#else
uint32_t irBuffer[50]; //infrared LED sensor data
uint32_t redBuffer[50];  //red LED sensor data
#endif

int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

char bufoxi[4]; // Con 100 caracteres debe haber más que suficiente para almacenar la cadena a enviar

void setup()
{
  Serial.begin(115200); // initialize serial communication at 115200 bits per second:
  mySerial.begin(115200);
  pinMode(PIN_beep , OUTPUT);  //definir pin como salida
  beep_lec=0;
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    //Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  ////<--------------
  
//  Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));
//  while (Serial.available() == 0) ; //wait until user presses a key
//  Serial.read();
  byte ledBrightness = 40; //Options: 0=Off to 255=50mA /60
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  //particleSensor.setup(55, 4, 2, 200, 411, 4096); //Configure sensor with these settings
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  iniciotiempo=millis();
  inicioprograma=millis();
  //----Ultrasonido
  pinMode(pinTrigger, OUTPUT); //Configuramoms el pin de "trigger" como salida
  pinMode(pinEcho, INPUT);  //Configuramoms el pin de "echo" como entrada
  digitalWrite(pinTrigger, LOW);//Ponemos en voltaje bajo(0V) el pin de "trigger"
  digitalWrite(pinGel, LOW);//Ponemos en voltaje Alto el pin de "Gel"
  
}


void loop()
{
  int ceros;
  myint=0;
  media_R=0;
  oxi=R;
  puls=0;
  ceros=1;
  //ULTRASONIDO
  unsigned long t,nm; //Variable de tipo unsigned long que contendrá el tiempo que le toma a la señal ir y regresar
  float d; //Variable de tipo float que contendrá la distancia en cm
  nm=0;
  flag_gel=0;
  while (1)
  {
    //si no esta conectado
    if(particleSensor.getRed()<70000){
//      Serial.print("0");
//      Serial.print(",");
//      Serial.print("0");
//      Serial.print(",");
//      Serial.print("0");
      beep_lec=0;
      if(ceros) {
        Serial.println("000%");
        mySerial.write("000%");
        ceros=0;
      }
      myint=0;
    }
    else{
      //dc removal del led rojo
      redBuffer[1] = particleSensor.getRed();
      contadorrms+=1;
      wprev=w;
      w=redBuffer[1]+(wprev*0.8);//0.01-0.1 preferentemente //regresa mas rapido que 0.95
      y=w-wprev;
      //suavizado
      Signal=float(y);
      S=(alpha*Signal)+((1-alpha)*S);

      irBuffer[1]=particleSensor.getIR();
      irwprev=irw;
      irw=irBuffer[1]+(irwprev*0.8);//0.01-0.1
      iry=irw-irwprev;
      //suavizado
      irSignal=float(iry);
      irS=(alpha*irSignal)+((1-alpha)*irS);

      //contador de pulsos en platismografia, algoritmo diferente al GIAcalibradooriginal
      if( (S>0)&&(band==0) ){
        contadorpulsos+=1;
        band=1;
      }
      if( (S<0)&&(band==1) ){
        contadorpulsos+=1;
        band=0;
      }
      //algoritmo antes de los 10segundos
      /*
      
      */
      //formato gia
      if((millis()-inicioprograma)<1000){
        //Serial.print(0);
      }
      else{
        //ceros=1;
        media_R+=R;
        myint++;
        puls+=heartrate;
        if(myint==media_lec){
          ceros=1;
          myint=0;
          media_R=media_R/media_lec;
          puls=puls/media_lec;
          
//          Serial.print(S);
//          Serial.print(",");
//          Serial.print(int (puls));
//          Serial.print(",");
          oxi=floor(115-16*R);
          if(oxi==0) {}
          else if(oxi<100) {
            //Serial.print(oxi);
            sprintf(bufoxi,"%03d",oxi);
            Serial.print(bufoxi);
            Serial.print("%");
            mySerial.print(String(bufoxi) + "%");
            //mySerial.print("%");
          }
          else Serial.print("099%");
                    
          if(beep_lec>=3){
            digitalWrite(PIN_beep , HIGH);   // poner el Pin en HIGH
            delay(500);                   // esperar
            digitalWrite(PIN_beep , LOW);    // poner el Pin en LOW
            if(beep_lec>=4) myint=media_lec-(media_lec/5);
            beep_lec++;
          }
          else beep_lec++;
        }//end media
      }
      if((millis()-iniciotiempo)>=5000){
        if(banderainicio==0){
          heartrate= (contadorpulsos*19)/2;//19
          heartrateant=heartrate;
          banderainicio=1;
        }
        else{
          //heartrate=((contadorpulsos*6)+heartrateant)/2;
          heartrate=(((contadorpulsos*12.2)/2)+heartrateant)/2;
          heartrateant=heartrate;
        }
        iniciotiempo=millis();
        contadorpulsos=0;
        //calculo spo2
        rms=sqrt(rms/contadorrms);
        irms=sqrt(irms/contadorrms);
        R=log(rms)/log(irms);
        contadorrms=0;
        rms=0;
        irms=0;
      }
      rms=rms+pow(S,2);
      irms=irms+pow(irS,2);
      }
//  //ULTRASONIDO
//  unsigned long t; //Variable de tipo unsigned long que contendrá el tiempo que le toma a la señal ir y regresar
//  float d; //Variable de tipo float que contendrá la distancia en cm

  nm++;
  if(nm>=2){
    digitalWrite(pinTrigger, HIGH);//Ponemos en voltaje alto(5V) el pin de "trigger"
    delayMicroseconds(10);//Esperamos en esta línea para conseguir un pulso de 10us
    digitalWrite(pinTrigger, LOW);//Ponemos en voltaje bajo(0V) el pin de "trigger"
    
    t = pulseIn(pinEcho, HIGH);         //Utilizamos la función  pulseIn() para medir el tiempo del pulso/echo
    d = t * 0.000001 * 34300.0 / 2.0; //Obtenemos la distancia considerando que la señal recorre dos veces la distancia a medir y que la velocidad del sonido es 343m/s
    if(flag_gel) {
      ngel++;
      if(ngel>20) {
        flag_gel=0;
      }
    }
    if( (d<=dis_detec)&&(!flag_gel) ){
      //Serial.print("D: ");
      //Serial.print(d);
      //Serial.println("cm");
      flag_gel=1;
      ngel=0;
      digitalWrite(pinGel, HIGH);//Ponemos en voltaje Alto el pin de "Gel"
      delay(50);
      digitalWrite(pinGel, LOW);//Ponemos en voltaje Alto el pin de "Gel"
    }//end distancia menor
    nm=0;
  }
  
  }//end while(1)
}

#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <TimerOne.h>

#define SS    10
#define RST   9
#define DIO0  2
#define pulsador1 A1
#define pulsador2 A2
bool estadoAnterior= HIGH;

#define SCREEN_WIDTH 128 // Ancho en píxeles de pantalla OLED
#define SCREEN_HEIGHT 64 // Altura en píxeles de pantalla OLED
int tammensaje=0;
float t=0,h=0,CO2=0;
String mensaje=0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //Declaración de pantalla I2C


// 1. Definir los estados
typedef enum {
  ESPERAR,
  EMISION,
  RECEPCION,
  ACCIONAR

} EstadoTransmisor;


EstadoTransmisor estadoActual;


void InicializarMEF() {
  estadoActual = ESPERAR;
}

String LoRaRx() {


 int packetSize = LoRa.parsePacket();

if (packetSize) {

    String msj = "";

  while (LoRa.available()) {
  tammensaje++;
      msj += (char)LoRa.read();
  }

  if(tammensaje==packetSize){
    Serial.print("Mensaje recibido: ");
    Serial.println(msj);
    return msj;
  }


  }
}


void LoRaTx() {

      delay(2000);  // espera 2 segundos

      Serial.println("Enviando mensaje...");
      LoRa.beginPacket();
      LoRa.print("Hola desde el transmisor\n");
      LoRa.endPacket();

 
}

void OledDisplay(char msj){

 display.clearDisplay();
  display.setTextSize(1,1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temperatura:");
  display.setCursor((73), 0);
  display.println(t);
  display.setCursor(0, 20);
  display.println("Humedad:");
  display.setCursor((48), 20);
  display.println(h);
  display.setCursor(0, 40);
  display.println("Nivel de C02:");
  display.setCursor((78), 40);
  display.println(CO2);
  display.display();

}

void setup() {

  Serial.begin(9600);

//-------------------------

  pinMode(pulsador1, INPUT_PULLUP);
 pinMode(pulsador2, INPUT_PULLUP);


//-------------------------


   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 
    Serial.println(F("OLED Falla"));
    while (1);
  }

  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Fallo al iniciar LoRa");
    while (1);
  }

  LoRa.setTxPower(20);
  Serial.println("LoRa listo");

  InicializarMEF();  // <--- inicializa los estados
}

void loop(){
  switch (estadoActual) {

    case EMISION:

    break;


    case RECEPCION:
    LoRaRx();
    OledDisplay();
    break;


    case ESPERAR:

    break;










    case ACCIONAR:
if (h>=10/*o cierta cantidad*/) {
display.clearDisplay();
  display.setTextSize(1,1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
 display.println ("puede accionar extractor");

 bool estadoActualBoton = digitalRead(pulsador1);

if (estadoAnterior == HIGH && estadoActualBoton == LOW) {
  Serial.println("Botón presionado. Enviando comando...");
  LoRa.beginPacket();
  LoRa.print("ENCENDER");
  LoRa.endPacket();
  Serial.println("Mensaje enviado: ENCENDER");
}

estadoAnterior = estadoActualBoton;  // actualizar para la próxima lectura
      }
break;
 }







//VERIFICAR SI ESTAN BIEN LAS LLAVES Y SI NO FALTA NINGUNA





  
LoRaTx();
OledDisplay();
}


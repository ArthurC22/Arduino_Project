#include <SoftwareSerial.h>

#define RxD 10    //Pin 10 pour arduino RX --> vers TxD de la carte Bluetooth
#define TxD 11   //Pin 11 pour arduino TX --> vers RxD de la carte Bluetooth
#define ThermistorPin 1 // Pin 1 pour capteur de température

SoftwareSerial BTSerie(RxD,TxD);

int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03;
float c2 = 2.378405444e-04;
float c3 = 2.019202697e-07;

void setup() {

  char c;

  InitCommunicationSerie() ;
  Serial.println("Initialisation carte : Ok");
  
  // Configuration du bluetooth
  pinMode(RxD, INPUT);  
  pinMode(TxD, OUTPUT);
  
  InitCommunicationBluetoothSerie();

  while (BTSerie.available()) {
    c = BTSerie.read();
    Serial.print(c);
  }
}

void loop() {
  char c;
  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    c = Serial.read();
    BTSerie.print(c);
  }
  while (BTSerie.available()) {
    c = BTSerie.read();
    if(c == 't'){
      LectureTemperature();
    }
    else{
      Serial.print(c);
    }
  }
}


//--------------------------------
// Initialisation de la communication serie avec l'ordinateur

void InitCommunicationSerie() {
  Serial.begin(9600);    
  while (!Serial) {
  }
  Serial.println("Demarrage connexion serie : Ok");
}


void InitCommunicationBluetoothSerie() {
  BTSerie.begin(9600);  //9600 den mode normal / 38400 en mode commande
  while (!BTSerie) {
      Serial.println("Attente reponse bluetooth");
  }
  Serial.println("Demarrage connexion bluetooth serie : Ok");
}

void LectureTemperature(){
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;

  BTSerie.print("Temperature: "); 
  BTSerie.print(T);
  BTSerie.println(" C");

  delay(500);
}

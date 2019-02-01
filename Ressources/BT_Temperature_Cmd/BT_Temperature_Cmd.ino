// Basé sur le code de LadyAda traduit et modifie par moi, domaine public
#include "DHT.h"
#define DHTPIN 8 // broche ou l'on a branche le capteur
// de-commenter le capteur utilise
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302)
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);//déclaration du capteur

#include <SoftwareSerial.h>

#define RxD 10    //Pin 10 pour arduino RX --> vers TxD de la carte Bluetooth
#define TxD 11   //Pin 11 pour arduino TX --> vers RxD de la carte Bluetooth
#define cmdPin 6 // Pin 6 pour le mode commande du module Bluetooth
#define ThermistorPin 1 // Pin 1 pour capteur de température

SoftwareSerial BTSerie(RxD,TxD);

boolean stateCmd;
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
  pinMode(cmdPin, OUTPUT);
  
  digitalWrite(cmdPin, LOW);
  stateCmd = false;   
  Serial.println("Mode Commande : OFF");
  
  InitCommunicationBluetoothSerie();

  while (BTSerie.available()) {
    c = BTSerie.read();
    Serial.print(c);
  }
  dht.begin();
}

void loop() {
  char c;
  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    c = Serial.read();
    if(c == 'c'){
      ChangeStateCmd();
    }
    else{
      BTSerie.print(c);
    }
  }
  while (BTSerie.available()) {
    c = BTSerie.read();
    if(c == 't'){
      LectureTemperature();
    }
    else if(c == 'h'){
      LectureHumidite();
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
  /*Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;

  BTSerie.print("Temperature: "); 
  BTSerie.print(T);
  BTSerie.println(" C");

  delay(500);*/
  float t = dht.readTemperature();//on lit la temperature en celsius (par defaut)
  // pour lire en farenheit, il faut le paramère (isFahrenheit = true) :
  float f = dht.readTemperature(true);
  if (isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  BTSerie.print("Temperature: ");
  BTSerie.print(t);
  BTSerie.print(" *C\n ");
}

void LectureHumidite(){
  float h = dht.readHumidity();//on lit l'hygrometrie
  if (isnan(h))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  BTSerie.print("Humidite: ");
  BTSerie.print(h);
  BTSerie.print(" %\n");
}

void ChangeStateCmd(){
  if(stateCmd == false){
    stateCmd = true;
    Serial.println("Mode Commande : ON");
    digitalWrite(cmdPin, HIGH);
  }
  else if(stateCmd == true){
    stateCmd = false;
    Serial.println("Mode Commande : OFF");
    digitalWrite(cmdPin, LOW);
  }
}


 



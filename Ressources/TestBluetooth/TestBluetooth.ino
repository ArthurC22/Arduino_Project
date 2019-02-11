#include <SoftwareSerial.h>

#define RxD 10    //Pin 10 pour arduino RX --> vers TxD de la carte Bluetooth
#define TxD 11   //Pin 11 pour arduino TX --> vers RxD de la carte Bluetooth

SoftwareSerial BTSerie(RxD,TxD);

void setup() {

  char c;

  InitCommunicationSerie() ;
  Serial.println("Initialisation carte : Ok");


  // Configuration du bluetooth
  pinMode(RxD, INPUT);  
  pinMode(TxD, OUTPUT);  
  
  InitCommunicationBluetoothSerie();

  BTSerie.print("AT+VERSION?");
  BTSerie.print("\r\n");

  Serial.println("Demande version ?");

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
    Serial.print(c);
  }
}


//--------------------------------
// Initialisation de la communication serie avec l'ordinateur

void InitCommunicationSerie() {
  Serial.begin(38400);    
  while (!Serial) {
  }
  Serial.println("Demarrage connexion serie : Ok");
}


void InitCommunicationBluetoothSerie() {
  BTSerie.begin(38400);  //9600 en mode normal / 38400 en mode commande
  while (!BTSerie) {
      Serial.println("Attente reponse bluetooth");
  }
  Serial.println("Demarrage connexion bluetooth serie : Ok");
}

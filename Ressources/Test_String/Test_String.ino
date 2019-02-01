#include <SoftwareSerial.h>

#define RxD 10    //Pin 10 pour arduino RX --> vers TxD de la carte Bluetooth
#define TxD 11   //Pin 11 pour arduino TX --> vers RxD de la carte Bluetooth

SoftwareSerial BTSerie(RxD,TxD);

void setup() {

  char c;

  InitCommunicationSerie() ;
  //Serial.println("Initialisation carte : Ok");
  
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
  char cmd[256];
  cmd[0] = '\0';
  int i = 0;
  int j = 0;

  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    c = Serial.read();
    cmd[i] = c;
    BTSerie.print(c);
    Serial.print(cmd);
    Serial.print("\n");
    i++;
  }

  cmd[i] = '\0';
  Serial.print(strcmp(cmd,"hello"));
  
  while (BTSerie.available()) {
    c = BTSerie.read();
    Serial.print(c);
  }

  if(strcmp(cmd,"bonjour") == 0){
    Serial.print("salut, poto !\n");
  }
}


//--------------------------------
// Initialisation de la communication serie avec l'ordinateur

void InitCommunicationSerie() {
  Serial.begin(9600);    
  while (!Serial) {
  }
  //Serial.println("Demarrage connexion serie : Ok");
}


void InitCommunicationBluetoothSerie() {
  BTSerie.begin(9600);  //9600 den mode normal / 38400 en mode commande
  while (!BTSerie) {
      //Serial.println("Attente reponse bluetooth");
  }
  //Serial.println("Demarrage connexion bluetooth serie : Ok");
}

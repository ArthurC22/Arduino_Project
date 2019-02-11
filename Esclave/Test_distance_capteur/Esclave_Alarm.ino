#include <SoftwareSerial.h>

/* Constantes pour les broches */
const byte TRIGGER_PIN = 3; // Broche TRIGGER
const byte ECHO_PIN = 2;    // Broche ECHO
const byte RxD = 10;   //Pin 12 pour arduino RX --> vers TxD de la carte Bluetooth
const byte TxD = 11;   //Pin 13 pour arduino TX --> vers RxD de la carte Bluetooth

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

SoftwareSerial BTSerie(RxD,TxD);

void setup() {
  // put your setup code here, to run once:
  char c;

  InitCommunicationSerie() ;
  Serial.println("Initialisation carte : Ok");
   
  /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);
  pinMode(RxD, INPUT);  
  pinMode(TxD, OUTPUT);

  // Configuration du bluetooth
  
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
  // put your main code here, to run repeatedly:
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
   
  /* Affiche les résultats en mm, cm et m */
  BTSerie.print(distance_mm / 10.0, 2);

  if (BTSerie.available())
    Serial.write(BTSerie.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerie.write(Serial.read());
   
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
  delay(500);
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

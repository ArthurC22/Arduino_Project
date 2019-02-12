bool setAlarme = true; //Booléen pour vérifier si l'alarme est activée

void setup() {
  Serial.println("Initialisation carte : ok");
  InitCommunicationSerie();
  InitCommunicationBluetoothPhone();
  InitCommunicationBluetoothSlave();
  pinMode(12, OUTPUT); //La pin connectée à la LED
}

void loop() {
  char c;
  char tab[50];
  int j = 0;
  int dist = 0;
  if(Serial2.available()){ //On écoute l'esclave (capteur de proximité)
    //Serial.println("Reception de : ");
    j = 0;
    while(Serial2.available()){
      c = Serial2.read();
      if(j<50){ // Ici on remplie un tableau afin de connaître la distance de la carte
        tab[j] = c;
        j++;
      }
      //Serial.print(c);
    }
    dist = atoi(tab); // On converti le tableau en int
    if(dist<100 && setAlarme){ // Si l'alarme est activé et que la distance est inférieure à 1m
      Serial.println("Alarme");
      digitalWrite(12, HIGH); // On allume la LED
    }
  }
  if(Serial.available()){ // Permet d'envoyer des messages aux appareils connectés (non utilisé)
    Serial.println("Envoie vers Bluetooth : ");
    while(Serial.available()){
      c=Serial.read();
      Serial.print(c);
      Serial2.print(c);
      Serial1.print(c);
    }
  }
  if (Serial1.available()) { // Réception du téléphone, permet d'activer et désactiver l'alarme
    Serial.println("Reception du telephone : ");  
    while(Serial1.available()){
      c = Serial1.read();  
      if(c == 'a'){
        setAlarme = true;
      }
      if(c == 'd'){  
        digitalWrite(12, LOW); 
        setAlarme = false;
      }
      Serial.print(c);
    }
  }
  delay(500); // Le délai permet d'obtenir correctement la distance envoyé par l'esclave
}

void InitCommunicationSerie(){
  Serial.begin(9600);
  while(!Serial){
  }
  Serial.println("Demarrage connexion serie : ok");
}

void InitCommunicationBluetoothSlave(){
  Serial2.begin(9600);
  while(!Serial2){
    Serial.println("Attente reponse bluetooth");
  }
  Serial.println("Demarrage connexion bluetooth slave : Ok");
}

void InitCommunicationBluetoothPhone(){
  Serial1.begin(57600);
  while(!Serial1){
    Serial.println("Attente reponse bluetooth");
  }
  Serial.println("Demarrage connexion bluetooth phone : Ok");
}

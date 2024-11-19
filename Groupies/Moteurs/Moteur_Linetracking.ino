#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(3); // Moteur gauche sur M3
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(4); // Moteur gauche sur M4

// Définition des broches
#define CAPTEUR_GAUCHE 2
#define CAPTEUR_DROIT 3


void setup() {
  pinMode(CAPTEUR_GAUCHE, INPUT);
  pinMode(CAPTEUR_DROIT, INPUT);

  Serial.begin(9600); // Pour debug
  Serial.println("Test du Shield et moteur");
  AFMS.begin(); // Initialiser le shield
}

void loop() {
  // Lecture des capteurs
  bool gauche = digitalRead(CAPTEUR_GAUCHE);
  bool droit = digitalRead(CAPTEUR_DROIT);
  myMotor1->setSpeed(150);                // Définition de la vitesse du moteur 3 à 150
  myMotor2->setSpeed(150);                // Définition de la vitesse du moteur 4 à 150

  // Debug : Affiche l'état des capteurs
  Serial.print("Gauche: ");
  Serial.print(gauche);
  Serial.print(" | Droit: ");
  Serial.println(droit);

  // Logique de suivi de ligne
  if (gauche && droit) {
    // Les deux capteurs détectent la ligne noire → Avancer
    myMotor1->run(FORWARD);                   // Aller vers l'avant
    myMotor2->run(FORWARD);                   // Aller vers l'avant
  } else if (!gauche && droit) {
    // Gauche sur la ligne noire, droit hors ligne → Tourner à gauche
    myMotor1->run(FORWARD);                   // Aller vers l'avant
    myMotor2->run(RELEASE);                   // S'arrête
  } else if (gauche && !droit) {
    // Droit sur la ligne noire, gauche hors ligne → Tourner à droite
    myMotor1->run(RELEASE);                   // S'arrête
    myMotor2->run(FORWARD);                   // Aller vers l'avant
  } else {
    // Les deux capteurs sont hors de la ligne noire → Stop
    myMotor1->run(RELEASE);                   // S'arrête
    myMotor2->run(RELEASE); 
  }
}



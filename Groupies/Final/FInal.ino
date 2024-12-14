#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <NewPing.h>
#include <Servo.h>

// Moteurs
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotorLEFT = AFMS.getMotor(3);
Adafruit_DCMotor *myMotorRIGHT = AFMS.getMotor(4);

// Capteurs IR
#define IR_LEFT A0
#define IR_RIGHT A1

// Capteur ultrason
#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 200
NewPing DistanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Servo
Servo servo;
#define SERVO_PIN 3
int spoint = 90; // Pos du servo

// PID
float kp = 1.5, ki = 0.0, kd = 0.5;
float previousError = 0, integral = 0;
int baseSpeed = 50;

// Suivi de distance
volatile int ticksGauche = 0; // Ticks pour le moteur gauche
volatile int ticksDroite = 0; // Ticks pour le moteur droit
float distanceParcourue = 0;
float distanceTotaleAvantVirage = 1100;
float distanceApresVirage = 200;
float circonferenceRoue = 70 * PI; //roue de 70 mm de diamètre (à ajuster)
int ticksParRotation = 20; //Ticks par rotation de l'encodeur (à ajuster)

// detect d'obstacles
const int obstacleThreshold = 15;

// Variables globales
int L = 0, R = 0, distance = 0;

// Encodeurs
#define ENCODER_LEFT_A 2
#define ENCODER_RIGHT_A 3

void setup() {
    Serial.begin(9600);
    AFMS.begin();

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);

    pinMode(ENCODER_LEFT_A, INPUT_PULLUP);
    pinMode(ENCODER_RIGHT_A, INPUT_PULLUP);

    // Attach interruptions pour compter les ticks
    attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A), countTicksGauche, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A), countTicksDroite, RISING);

    servo.attach(SERVO_PIN);
    servo.write(spoint); // Position initiale (centrale)
    delay(500);
}

void loop() {
    // Calcul de la distance parcourue
    updateDistance();

    // Gestion des étapes
    if (distanceParcourue < distanceTotaleAvantVirage) {
        avancer();
    } else if (distanceParcourue < distanceTotaleAvantVirage + distanceApresVirage) {
        if (distanceParcourue == distanceTotaleAvantVirage) {
            //virage de 45°
            virage(45);
        }
        avancer();
    } else {
        Stop();
        return;
    }

    // Vérification des obstacles
    distance = DistanceSensor.ping_cm();
    if (distance > 0 && distance <= obstacleThreshold) {
        Obstacle();
        return;
    }
}

float computePID(float error) {
    integral += error;
    float derivative = error - previousError;
    previousError = error;
    return kp * error + ki * integral + kd * derivative;
}

void Obstacle() {
    Stop();
    backward();
    delay(500);
    Stop();

    servo.write(45); // Vers la gauche
    delay(800);
    L = DistanceSensor.ping_cm();

    servo.write(135); // Vers la droite
    delay(800);
    R = DistanceSensor.ping_cm();

    servo.write(spoint); // Retour au centre
    delay(500);

    if (L < R) {
        right();
        delay(500);
        Stop();
        delay(200);
    } else if (L > R) {
        left();
        delay(500);
        Stop();
        delay(200);
    } else {
        forward();
    }
}

void avancer() {
    myMotorLEFT->setSpeed(baseSpeed);
    myMotorRIGHT->setSpeed(baseSpeed);
    myMotorLEFT->run(FORWARD);
    myMotorRIGHT->run(FORWARD);
}

void backward() {
    myMotorLEFT->setSpeed(baseSpeed);
    myMotorRIGHT->setSpeed(baseSpeed);
    myMotorLEFT->run(BACKWARD);
    myMotorRIGHT->run(BACKWARD);
}

void right() {
    myMotorLEFT->setSpeed(baseSpeed);
    myMotorRIGHT->setSpeed(baseSpeed);
    myMotorLEFT->run(FORWARD);
    myMotorRIGHT->run(BACKWARD);
}

void left() {
    myMotorLEFT->setSpeed(baseSpeed);
    myMotorRIGHT->setSpeed(baseSpeed);
    myMotorLEFT->run(BACKWARD);
    myMotorRIGHT->run(FORWARD);
}

void Stop() {
    myMotorLEFT->setSpeed(0);
    myMotorRIGHT->setSpeed(0);
    myMotorLEFT->run(RELEASE);
    myMotorRIGHT->run(RELEASE);
}

void virage(int angle) {
    // Effectuer un virage en fonction de l'angle
    myMotorLEFT->setSpeed(baseSpeed);
    myMotorRIGHT->setSpeed(baseSpeed);
    myMotorLEFT->run(FORWARD);
    myMotorRIGHT->run(BACKWARD);
    delay(angle * 10); //durée pour virage (à ajuster)
    Stop();
}

void updateDistance() {
    noInterrupts(); // Empêche les interruptions pendant la lecture
    int ticksGaucheCopy = ticksGauche;
    int ticksDroiteCopy = ticksDroite;
    interrupts(); // Réactive les interruptions

    float moyenneTicks = (ticksGaucheCopy + ticksDroiteCopy) / 2.0;
    distanceParcourue += moyenneTicks * (circonferenceRoue / ticksParRotation);

    // Réinitialiser les ticks pour la prochaine itération
    ticksGauche = 0;
    ticksDroite = 0;
}

void countTicksGauche() {
    ticksGauche++;
}

void countTicksDroite() {
    ticksDroite++;
}

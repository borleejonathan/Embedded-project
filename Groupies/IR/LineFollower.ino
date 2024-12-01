#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotorLEFT = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorRIGHT = AFMS.getMotor(3);

int M1_Speed = 150; 
int M2_Speed = 150;

void setup() {
    Serial.begin(9600);
    AFMS.begin();

    pinMode(A0, INPUT); // Capteur IR gauche
    pinMode(A1, INPUT); // Capteur IR droit
}

void loop() {
    int LEFT_SENSOR = digitalRead(A0);
    int RIGHT_SENSOR = digitalRead(A1);

    if (RIGHT_SENSOR == 0 && LEFT_SENSOR == 0) {
        forward();
    } 
    else if (RIGHT_SENSOR == 0 && LEFT_SENSOR == 1) {
        left();
    } 
    else if (RIGHT_SENSOR == 1 && LEFT_SENSOR == 0) {
        right();
    } 
    else if (RIGHT_SENSOR == 1 && LEFT_SENSOR == 1) {
        Stop();
    }
}

void forward() {
    myMotorLEFT->setSpeed(M1_Speed);
    myMotorRIGHT->setSpeed(M2_Speed);
    myMotorLEFT->run(FORWARD);
    myMotorRIGHT->run(FORWARD);
}

void backward() {
    myMotorLEFT->setSpeed(M1_Speed);
    myMotorRIGHT->setSpeed(M2_Speed);
    myMotorLEFT->run(BACKWARD);
    myMotorRIGHT->run(BACKWARD);
}

void right() {
    myMotorLEFT->setSpeed(M1_Speed);
    myMotorRIGHT->setSpeed(M2_Speed);
    myMotorLEFT->run(FORWARD);
    myMotorRIGHT->run(BACKWARD);
}

void left() {
    myMotorLEFT->setSpeed(M1_Speed);
    myMotorRIGHT->setSpeed(M2_Speed);
    myMotorLEFT->run(BACKWARD);
    myMotorRIGHT->run(FORWARD);
}

void Stop() {
    myMotorLEFT->setSpeed(0);
    myMotorRIGHT->setSpeed(0);
    myMotorLEFT->run(RELEASE);
    myMotorRIGHT->run(RELEASE);
}
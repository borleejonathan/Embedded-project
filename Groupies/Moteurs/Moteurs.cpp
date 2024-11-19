#include "Motors.h"

// Vars
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motorLeft = AFMS.getMotor(2);
Adafruit_DCMotor *motorRight = AFMS.getMotor(1);

volatile long leftEncCount = 0;
volatile long rightEncCount = 0;

const int leftEncPin = 2;
const int rightEncPin = 3;

int corrFactor = 0;
int turnStr = 50;

State currState = INIT; // FSM

// Init motors + encs
void setupMotors() {
    AFMS.begin();
    pinMode(leftEncPin, INPUT_PULLUP);
    pinMode(rightEncPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(leftEncPin), countLeftEnc, FALLING);
    attachInterrupt(digitalPinToInterrupt(rightEncPin), countRightEnc, FALLING);
}

// Move fwd/bwd
void move(int spd) {
    if (spd >= 0) {
        motorLeft->setSpeed(spd);
        motorRight->setSpeed(spd - corrFactor);
        motorLeft->run(FORWARD);
        motorRight->run(FORWARD);
    } else {
        motorLeft->setSpeed(-spd);
        motorRight->setSpeed(-spd - corrFactor);
        motorLeft->run(BACKWARD);
        motorRight->run(BACKWARD);
    }
}

// Smooth turn
void turn(Direction dir, int spd) {
    if (dir == right) {
        int rSpd = spd * (100 - turnStr) / 100;
        motorLeft->setSpeed(spd);
        motorRight->setSpeed(rSpd - corrFactor);
    } else {
        int lSpd = spd * (100 - turnStr) / 100;
        motorLeft->setSpeed(lSpd);
        motorRight->setSpeed(spd - corrFactor);
    }
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
}

// Turn in place
void turnInPlace(Direction dir, int spd) {
    if (dir == right) {
        motorLeft->setSpeed(spd);
        motorRight->setSpeed(spd - corrFactor);
        motorLeft->run(FORWARD);
        motorRight->run(BACKWARD);
    } else {
        motorLeft->setSpeed(spd);
        motorRight->setSpeed(spd - corrFactor);
        motorLeft->run(BACKWARD);
        motorRight->run(FORWARD);
    }
}

// Stop motors
void stopMotors() {
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);
}

// Enc ISR
void countLeftEnc() { leftEncCount++; }
void countRightEnc() { rightEncCount++; }

// Print enc counts
void printEnc(const char* seq) {
    long diff = leftEncCount - rightEncCount;
    Serial.print(seq);
    Serial.println(" :");
    Serial.print("L: ");
    Serial.println(leftEncCount);
    Serial.print("R: ");
    Serial.println(rightEncCount);
    Serial.print("D: ");
    Serial.println(diff);
    leftEncCount = 0;
    rightEncCount = 0;
}

// FSM
void handleState() {
    switch (currState) {
        case INIT:
            stopMotors();
            delay(1000);
            currState = FWD;
            break;

        case FWD:
            move(200);
            delay(2000);
            printEnc("Fwd");
            currState = TURN_L;
            break;

        case TURN_L:
            turn(left, 150);
            delay(2000);
            printEnc("Turn L");
            currState = BWD;
            break;

        case BWD:
            move(-200);
            delay(2000);
            printEnc("Bwd");
            currState = TURN_R;
            break;

        case TURN_R:
            turn(right, 150);
            delay(2000);
            printEnc("Turn R");
            currState = STOP;
            break;

        case STOP:
            stopMotors();
            delay(1000);
            currState = END;
            break;

        case END:
            stopMotors();
            while (true); // Halt
            break;
    }
}

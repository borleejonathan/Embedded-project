#include "motor_control.h"
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

void initMotors() {
    AFMS.begin();
    leftMotor->setSpeed(150);
    rightMotor->setSpeed(150);
}

void moveForward() {
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
}

void turnLeft() {
    leftMotor->run(BACKWARD);
    rightMotor->run(FORWARD);
}

void turnRight() {
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
}

void stopMotors() {
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
}

void turn90Left() {
    turnLeft();
    delay(500); // Adjust timing for 90° turn
    stopMotors();
}

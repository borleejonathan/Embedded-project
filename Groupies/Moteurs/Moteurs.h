#ifndef MOTORS_H
#define MOTORS_H

#include <Adafruit_MotorShield.h>
#include <Wire.h>

// Motors + vars
extern Adafruit_MotorShield AFMS;
extern Adafruit_DCMotor *motorLeft;
extern Adafruit_DCMotor *motorRight;

extern volatile long leftEncCount;
extern volatile long rightEncCount;

extern const int leftEncPin;
extern const int rightEncPin;

extern int corrFactor;
extern int turnStr;

// Enums
enum Direction { left, right };
enum State { INIT, FWD, BWD, TURN_L, TURN_R, STOP, END };

// Fns
void setupMotors();
void move(int spd);
void turn(Direction dir, int spd);
void turnInPlace(Direction dir, int spd);
void stopMotors();
void countLeftEnc();
void countRightEnc();
void printEnc(const char* seq);
void handleState();

#endif

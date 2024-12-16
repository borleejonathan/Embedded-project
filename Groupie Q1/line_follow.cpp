#include "line_follow.h"
#include "motor_control.h"

// Init IR sensors
void initIRSensors() {
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);
}

// Suivi de ligne
void followLine() {
    if (digitalRead(IR_LEFT) == LOW && digitalRead(IR_RIGHT) == LOW) {
        moveForward(); // Avancer tout droit
    } else if (digitalRead(IR_LEFT) == LOW) {
        turnRight();
    } else if (digitalRead(IR_RIGHT) == LOW) {
        turnLeft();
    }
}

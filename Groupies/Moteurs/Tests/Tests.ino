#include "../Moteurs.h"

void setup() {
    Serial.begin(9600);
    setupMotors();
}

void loop() {
    handleState();
}

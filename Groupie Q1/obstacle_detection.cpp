#include "obstacle_detection.h"
#include "config.h"

void initObstacleDetection() {
    pinMode(HC_TRIG, OUTPUT);
    pinMode(HC_ECHO, INPUT);
}

// Check if obstacle detected
bool obstacleDetected() {
    digitalWrite(HC_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(HC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(HC_TRIG, LOW);

    long duration = pulseIn(HC_ECHO, HIGH);
    long distance = duration * 0.034 / 2;

    return (distance < OBSTACLE_THRESHOLD);
}

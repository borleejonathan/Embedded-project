#include "timers.h"

unsigned long timerStartTimes[3];
unsigned long timerDurations[3];

void startTimer(int timer, int duration) {
    timerStartTimes[timer] = millis();
    timerDurations[timer] = duration;
}

bool timerExpired(int timer) {
    return (millis() - timerStartTimes[timer]) >= timerDurations[timer];
}

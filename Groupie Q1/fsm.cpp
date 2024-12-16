#include "fsm.h"
#include "motor_control.h"
#include "line_follow.h"
#include "obstacle_detection.h"
#include "bluetooth.h"
#include "led_animation.h"
#include "timers.h"

enum State { INIT, LINE_FOLLOW, STOP_T1, TURN_90, MOVE_T2, WAIT_T3, FINISH };
State currentState = INIT;

void initFSM() {
    currentState = INIT;
    sendBluetoothMessage("INIT"); // BT msg
}

void runFSM() {
    switch (currentState) {
        case INIT:
            if (buttonPressed()) {
                currentState = LINE_FOLLOW;
                startTimer(T1, T1_DURATION);
                sendBluetoothMessage("LINE_FOLLOW");
            }
            break;

        case LINE_FOLLOW:
            followLine();
            if (timerExpired(T1) || obstacleDetected()) {
                stopMotors();
                currentState = STOP_T1;
                sendBluetoothMessage("STOP_T1");
            }
            break;

        case STOP_T1:
            turn90Left();
            currentState = TURN_90;
            sendBluetoothMessage("TURN_90");
            break;

        case TURN_90:
            if (turnComplete()) {
                startTimer(T2, T2_DURATION);
                currentState = MOVE_T2;
                sendBluetoothMessage("MOVE_T2");
            }
            break;

        case MOVE_T2:
            moveForward();
            if (timerExpired(T2) || obstacleDetected()) {
                stopMotors();
                startTimer(T3, T3_DURATION);
                currentState = WAIT_T3;
                sendBluetoothMessage("WAIT_T3");
            }
            break;

        case WAIT_T3:
            if (timerExpired(T3)) {
                currentState = FINISH;
                sendBluetoothMessage("FINISH");
            }
            break;

        case FINISH:
            ledDance();
            break;
    }
}

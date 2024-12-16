#include "bluetooth.h"
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(HC_RX, HC_TX);

void initBluetooth() {
    bluetooth.begin(9600);
}

void sendBluetoothMessage(const char *message) {
    bluetooth.println(message);
}

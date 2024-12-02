#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);  //port M1
Adafruit_DCMotor *Motor2 = AFMS.getMotor(3);  //port M3

void setup() {
  AFMS.begin(); 


  Motor1->setSpeed(255);
  Motor2->setSpeed(255);
}

void loop() {
  Motor1->run(FORWARD);
  Motor2->run(FORWARD);
  delay(1000);

  Motor1->run(BACKWARD);
  Motor2->run(BACKWARD);
  delay(1000);

  Motor1->run(FORWARD);
  Motor2->run(BACKWARD);
  delay(1000);

  Motor1->run(BACKWARD);
  Motor2->run(FORWARD);
  delay(1000);

  Motor1->setSpeed(0);
  Motor2->setSpeed(0);
  Motor1->run(RELEASE);
  Motor2->run(RELEASE);
  delay(1000);
}
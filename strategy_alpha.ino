#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <ArduinoJson.h>  // Install via Library Manager
#include <SoftwareSerial.h>


unsigned long startTime = 0;
unsigned long endTime = 0;
bool bannerDeployed = false;
bool stepPushed = false;
bool finalZoneReached = false;

// --- Ultrasonic sensors (used for obstacle detection) ---

#define DISTANCE_THRESHOLD 15 // Obstacle detection threshold in cm

// --- OLED Screen ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int score = 0;
int lastAdded = 0;
unsigned long lastAddTime = 0;
const unsigned long showBonusDuration = 1500; // Time to show "+X" on screen

// --- Servo Motors ---
Servo locker;
Servo deploy;
Servo frontlocker;

// --- DC Motors Pins ---
int motor1pin1 = 22;
int motor1pin2 = 23;
int motor2pin1 = 24;
int motor2pin2 = 25;
int motor3pin1 = 26;
int motor3pin2 = 27;


// --- Encodeurs ---
const int leftEncA = 18;
const int leftEncB = 30;
const int rightEncA = 19;
const int rightEncB = 31;

volatile long ticksLeft = 0;
volatile long ticksRight = 0;

int starttime = 0;

void setup() {
  Serial.begin(9600);

    // Display waiting message
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Waiting for start...");
  display.println("Pull the tirette");
  display.display();
  Serial.println("Waiting for tirette (pin 35 HIGH)");

  starttime = millis();

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 25);
  display.println("GO!");
  display.display();

  delay(500);

  // Initialize OLED screen
  updateDisplay();


  // Set motor pins as output
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(motor3pin1, OUTPUT);
  pinMode(motor3pin2, OUTPUT);


  // Encodeurs
  pinMode(leftEncA, INPUT_PULLUP);
  pinMode(leftEncB, INPUT_PULLUP);
  pinMode(rightEncA, INPUT_PULLUP);
  pinMode(rightEncB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(leftEncA), readLeftEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightEncA), readRightEncoder, CHANGE);

  // Motor speed pins
  pinMode(11, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  delay(500);

  // Set motor speeds
  analogWrite(11, 200);
  analogWrite(5, 115); //110
  analogWrite(6, 170);


// -------------------------------------------------------//
// Phase 1: Banner deployment
// -------------------------------------------------------//
// Release the locker
  locker.attach(10);
  locker.write(0);
  delay(500);

// Deploy the banner system towards the back
  deploy.attach(2);
  deploy.write(70);
  delay(500);

// Move forward to get back inside the valid zone
  forward();
  delay(400);
  stop();

// Activate the banner motor one last time
  dcForward();
  deploy.write(90); delay(1500);
  deploy.write(70); delay(1500);
  deploy.write(90); delay(1500);
  delay(1000);
  dcStop();
  delay(1000);

// The pusher pushes the cylinder off the motor shaft
  pushForward();
  delay(400);
  pushStop();
  delay(500);

// Reset locker and deploy servos to initial position
  locker.write(180);
  deploy.write(45);
  delay(1000);
  pushBackward();
  delay(400);
  pushStop();
  delay(500);

  // -------------------------------------------------------//
  // Phase 2: Step placement
  // -------------------------------------------------------//
  // Move toward the step 
  while (ticksLeft < 2000 && ticksRight < 2000){
      forward();
  }

  rightrotation(); delay(650); resetTicks();

  while (ticksLeft < 1800 && ticksRight < 1800){
    forward();
  }

  rightrotation(); delay(700); 
  forward(); delay(600);

  // In front of the step, lock it
  frontlocker.attach(7); frontlocker.write(20); delay(500);

  // Push the step
  forward(); delay(300);

  // Unlock the step
  frontlocker.write(110); delay(1000);

  // Earn 4 points for placing the step
  addPoints(4);

  // -------------------------------------------------------//
  // Phase 3: Back to final zone
  // -------------------------------------------------------//
  backward(); delay(600);
  leftrotation(); delay(600); resetTicks();

  stop(); delay(100);
  leftrotation(); delay(600); 
  while (millis() - starttime < 92000){
    stop();
  }

  forward(); delay(100);resetTicks();
}

void loop() {
  // Clear bonus display after delay
  if (lastAdded != 0 && millis() - lastAddTime >= showBonusDuration) {
    lastAdded = 0;
    updateDisplay();
  }
  
  stop();
  addPoints(10);
  Serial.println("Mission terminée - arrêt long");
  delay(1000000); // End of behavior - wait for a long time  
} 


// === Score handling ===
void addPoints(int pts) {
  score += pts;
  lastAdded = pts;
  lastAddTime = millis();
  updateDisplay();
}

void updateDisplay() {
  display.clearDisplay();

  if (lastAdded != 0) {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(75, 0);
    display.print("+");
    display.print(lastAdded);
  }

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 30);
  display.print("Score: ");
  display.print(score);

  display.display();
}

// === Motor controls ===
void dcStop() {
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);
}

void dcForward() {
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, HIGH);
}
void pushForward() {
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, HIGH);
}

void pushBackward() {
  digitalWrite(motor3pin1, HIGH);
  digitalWrite(motor3pin2, LOW);
}
void pushStop() {
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);
}

// === Ultrasonic distance measurement ===
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}

// === Movement functions ===
void forward() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
}

void backward() {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
}

void stop() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
}

void rightrotation() {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
}

void leftrotation() {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
}
void readLeftEncoder() {
  bool a = digitalRead(leftEncA);
  bool b = digitalRead(leftEncB);
  if (a == b) {
    ticksLeft++;
  } else {
    ticksLeft--;
  }
}

void readRightEncoder() {
  bool a = digitalRead(rightEncA);
  bool b = digitalRead(rightEncB);
  if (a == b) {
    ticksRight--;
  } else {
    ticksRight++;
  }
}

void resetTicks() {
  ticksLeft = 0;
  ticksRight = 0;
}


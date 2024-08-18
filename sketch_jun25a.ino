#include <SoftwareSerial.h>
#include <Servo.h>

/** Set control port **/
const int E1Pin = 10;
const int M1Pin = 12;
const int E2Pin = 11;
const int M2Pin = 13;

/** Inner definition **/
typedef struct {
  byte enPin;
  byte directionPin;
} MotorControl;

const int M1 = 1;
const int M2 = 0;
const int MotorNum = 2;

const MotorControl MotorPin[] = { {E1Pin, M1Pin}, {E2Pin, M2Pin} };

const int Forward = LOW;
const int Backward = HIGH;

Servo servo1; // First servo
Servo servo2; // Second servo

int bttx = 7;  // tx of Bluetooth module is connected to pin 7 of Arduino
int btrx = 6;  // rx of Bluetooth module is connected to pin 6 of Arduino
SoftwareSerial bluetooth(bttx, btrx);

void setup() {
  pinMode(E1Pin, OUTPUT);
  pinMode(M1Pin, OUTPUT);
  pinMode(E2Pin, OUTPUT);
  pinMode(M2Pin, OUTPUT);
  digitalWrite(E1Pin, LOW);
  digitalWrite(M1Pin, LOW);
  digitalWrite(E2Pin, LOW);
  digitalWrite(M2Pin, LOW);

  servo1.attach(5);   // First servo is connected to pin 5
  servo2.attach(4);   // Second servo is connected to pin 4
  servo1.write(90);   // Initialize the first servo to the middle position
  servo2.write(110);   // Initialize the second servo to the middle position
  Serial.begin(9600);
  bluetooth.begin(9600);
  initMotor();

  stopAllMotors(); // Stop all motors at start
}

void loop() {
  if (bluetooth.available() > 0) { // If Bluetooth module is transmitting data
    char command = bluetooth.read(); // Read the data from Bluetooth module
    Serial.println(command);

    switch (command) {
      case 'a':
        startMotors_3(Forward, Forward);
        break;
      case 'd':
        startMotors_3(Backward, Backward);
        break;
      case 's':
        startMotors_2(Backward, Forward);
        break;
      case 'w':
        startMotors(Forward, Backward);
        break;
      case 'o':
        servo1.write(60); // Move first servo to position 0
        break;
      case 'n':
        servo1.write(20); // Move first servo to position 180
        break;
      case 'm': // Up shovel position
        servo2.write(0); // Move second servo to position 90
        break;
      case 'p': // Down shovel position
        servo2.write(120); // Move second servo to position 180
        break;
      case 'z':
        stopAllMotors();
        break;

      case 'k': // fucntion to shake the butt of the robot.
        shakeRobot();
        break;
    }
  }
}

void startMotors(int direction1, int direction2) {
  setMotorDirection(M1, direction1);
  setMotorDirection(M2, direction2);
  setMotorSpeed(M1, 100);
  setMotorSpeed(M2, 100);
}

void startMotors_2(int direction1, int direction2) {
  setMotorDirection(M1, direction1);
  setMotorDirection(M2, direction2);
  setMotorSpeed(M1, 30);
  setMotorSpeed(M2, 30);
}

void startMotors_3(int direction1, int direction2) {
  setMotorDirection(M1, direction1);
  setMotorDirection(M2, direction2);
  setMotorSpeed(M1, 50);
  setMotorSpeed(M2, 50);
}

void initMotor() {
  for (int i = 0; i < MotorNum; i++) {
    digitalWrite(MotorPin[i].enPin, LOW);
    pinMode(MotorPin[i].enPin, OUTPUT);
    pinMode(MotorPin[i].directionPin, OUTPUT);
  }
}

void stopAllMotors() {
  setMotorSpeed(M1, 0);
  setMotorSpeed(M2, 0);
  setMotorDirection(M1, Forward);  // Set initial direction to forward
  setMotorDirection(M2, Forward);  // Set initial direction to forward
}

void setMotorDirection(int motorNumber, int direction) {
  digitalWrite(MotorPin[motorNumber].directionPin, direction);
}

inline void setMotorSpeed(int motorNumber, int speed) {
  analogWrite(MotorPin[motorNumber].enPin, 255.0 * (speed / 100.0)); // PWM
}

void shakeRobot() {
  unsigned long startTime = millis();
  while (millis() - startTime < 3000) { // Shake for 3 seconds
    startMotors(Forward, Backward); // Run motors in opposite directions
    delay(200); // Wait for 200 milliseconds
    startMotors(Backward, Forward); // Run motors in opposite directions
    delay(200); // Wait for 200 milliseconds
  }
  stopAllMotors(); // Stop all motors after shaking
}


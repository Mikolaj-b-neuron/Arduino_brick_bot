// Define motor control pins
const int leftMotorForward = 5;  // Adjust based on your wiring
const int leftMotorBackward = 6; // Adjust based on your wiring
const int rightMotorForward = 9; // Adjust based on your wiring
const int rightMotorBackward = 10; // Adjust based on your wiring

char command = ' ';

void setup() {
  // Initialize motor control pins as outputs
  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);

  // Start the serial communication with the Bluetooth module
  Serial.begin(9600);
}

void loop() {
  // Check if a command is available from the Bluetooth module
  if (Serial.available()) {
    command = Serial.read();

    // Stop all motors before executing any new command
    stopMotors();

    switch (command) {
      case '1':  // Left motor forward
        digitalWrite(leftMotorForward, HIGH);
        digitalWrite(leftMotorBackward, LOW);
        break;

      case 'L':  // Left motor backward
        digitalWrite(leftMotorForward, LOW);
        digitalWrite(leftMotorBackward, HIGH);
        break;

      case '2':  // Right motor forward
        digitalWrite(rightMotorForward, HIGH);
        digitalWrite(rightMotorBackward, LOW);
        break;

      case 'R':  // Right motor backward
        digitalWrite(rightMotorForward, LOW);
        digitalWrite(rightMotorBackward, HIGH);
        break;

      case 'S':  // Stop both motors
      default:
        // Invalid command, stop all motors
        stopMotors();
        break;
    }
  }
}

void stopMotors() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

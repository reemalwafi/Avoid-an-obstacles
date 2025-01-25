#include <NewPing.h>      // Ultrasonic sensor library
#include <Servo.h>        // Servo motor library

const int leftMotorForwardPin = 6;
const int leftMotorBackwardPin = 7;
const int rightMotorForwardPin = 8;
const int rightMotorBackwardPin = 9;

// Sensor pins
#define trigPin A1
#define echoPin A2

#define maxDistance 200
boolean movingForward = false;
int currentDistance = 100;

NewPing sonar(trigPin, echoPin, maxDistance); // Sensor instance
Servo myServo; // Servo motor instance

void setup() {
  pinMode(rightMotorForwardPin, OUTPUT);
  pinMode(leftMotorForwardPin, OUTPUT);
  pinMode(leftMotorBackwardPin, OUTPUT);
  pinMode(rightMotorBackwardPin, OUTPUT);

  myServo.attach(10); // Attach the servo to pin 10

  myServo.write(115);
  delay(2000);
  currentDistance = getDistance();
  delay(100);
  currentDistance = getDistance();
  delay(100);
  currentDistance = getDistance();
  delay(100);
  currentDistance = getDistance();
  delay(100);
}

void loop() {
  int rightDist = 0;
  int leftDist = 0;
  delay(50);

  if (currentDistance <= 20) {
    stopMovement();
    delay(300);
    reverse();
    delay(400);
    stopMovement();
    delay(300);
    rightDist = scanRight();
    delay(300);
    leftDist = scanLeft();
    delay(300);

    if (currentDistance >= leftDist) {
      turnToRight();
      stopMovement();
    } else {
      turnToLeft();
      stopMovement();
    }
  } else {
    moveForward();
  }
  currentDistance = getDistance();
}

int scanRight() {
  myServo.write(50);  // Turn servo to the right
  delay(500);
  int distance = getDistance();
  delay(100);
  myServo.write(115); // Reset servo position
  return distance;
}

int scanLeft() {
  myServo.write(170); // Turn servo to the left
  delay(500);
  int distance = getDistance();
  delay(100);
  myServo.write(115); // Reset servo position
  return distance;
}

int getDistance() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

void stopMovement() {
  digitalWrite(rightMotorForwardPin, LOW);
  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, LOW);
  digitalWrite(leftMotorBackwardPin, LOW);
}

void moveForward() {
  if (!movingForward) {
    movingForward = true;

    digitalWrite(leftMotorForwardPin, HIGH);
    digitalWrite(rightMotorForwardPin, HIGH);

    digitalWrite(leftMotorBackwardPin, LOW);
    digitalWrite(rightMotorBackwardPin, LOW);
  }
}

void reverse() {
  movingForward = false;

  digitalWrite(leftMotorBackwardPin, HIGH);
  digitalWrite(rightMotorBackwardPin, HIGH);

  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(rightMotorForwardPin, LOW);
}

void turnToRight() {
  digitalWrite(leftMotorForwardPin, HIGH);
  digitalWrite(rightMotorBackwardPin, HIGH);

  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorForwardPin, LOW);

  delay(500);

  digitalWrite(leftMotorForwardPin, HIGH);
  digitalWrite(rightMotorForwardPin, HIGH);

  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, LOW);
}

void turnToLeft() {
  digitalWrite(leftMotorBackwardPin, HIGH);
  digitalWrite(rightMotorForwardPin, HIGH);

  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, LOW);

  delay(500);

  digitalWrite(leftMotorForwardPin, HIGH);
  digitalWrite(rightMotorForwardPin, HIGH);

  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, LOW);
}

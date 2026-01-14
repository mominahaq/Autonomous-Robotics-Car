
#include <AFMotor.h>
#include <NewPing.h>

// ---------------- MOTOR SETUP ----------------
AF_DCMotor motor1(1); // Motor 1
AF_DCMotor motor2(2); // Motor 2
AF_DCMotor motor3(3); // Motor 3
AF_DCMotor motor4(4); // Motor 4

#define MOTOR_SPEED 200  // PWM speed (0-255)

// ---------------- ULTRASONIC SENSOR ----------------
#define TRIG_PIN 8
#define ECHO_PIN 9
#define MAX_DISTANCE 200  // cm

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// ---------------- BATTERY MONITOR (optional) ----------------
#define BATTERY_PIN A3  // Voltage divider connected here
float batteryVoltage = 0;

// ---------------- FLAGS ----------------
bool wallReached = false;
bool startReached = false;
unsigned int startDistance = 0;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  // Initialize motors
  motor1.setSpeed(MOTOR_SPEED);
  motor2.setSpeed(MOTOR_SPEED);
  motor3.setSpeed(MOTOR_SPEED);
  motor4.setSpeed(MOTOR_SPEED);

  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BATTERY_PIN, INPUT);

  delay(2000); // wait 2 sec
  startDistance = readDistance();
  Serial.println("System Ready...");
}

// ---------------- LOOP ----------------
void loop() {
  // Read battery voltage
  readBattery();

  // Measure distance
  unsigned int distance = readDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  // Move forward until wall detected
  if (!wallReached) {
    if (distance > 2) {
      moveForward();
    } else {
      stopMotors();
      delay(1000);
      reverse();
      wallReached = true;
      Serial.println("Wall reached, reversing...");
    }
  } 
  // Check if starting point reached
  else if (!startReached) {
    if (distance >= startDistance) {
      stopMotors();
      startReached = true;
      Serial.println("Returned to start point.");
    } else {
      reverse();
    }
  }
}

// ---------------- FUNCTIONS ----------------
void moveForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void reverse() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

// Read distance from ultrasonic sensor
unsigned int readDistance() {
  unsigned int distance = sonar.ping_cm();
  if (distance == 0) distance = MAX_DISTANCE; // no echo
  return distance;
}

// Read battery voltage
void readBattery() {
  int sensorValue = analogRead(BATTERY_PIN);  // 0-1023
  // Assuming voltage divider divides 11.1V by 2
  batteryVoltage = sensorValue * (11.1 / 1023.0) * 2;
  Serial.print("Battery Voltage: ");
  Serial.println(batteryVoltage);
}

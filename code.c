#include <Servo.h>

// Pins for LEDs
const int lane1Red = 2;
const int lane1Green = 3;
const int lane2Red = 4;
const int lane2Green = 5;

// Pins for Ultrasonic Sensor
const int trigPin = 6;
const int echoPin = 7;

// Servo objects for barriers
Servo lane1Servo;
Servo lane2Servo;

// Distance threshold for pedestrian detection
const int detectionThreshold = 50; // in cm

// Servo positions
const int barrierOpen = 90;  // Open position
const int barrierClosed = 0; // Closed position

void setup() {
  // Initialize LEDs as outputs
  pinMode(lane1Red, OUTPUT);
  pinMode(lane1Green, OUTPUT);
  pinMode(lane2Red, OUTPUT);
  pinMode(lane2Green, OUTPUT);

  // Initialize Ultrasonic Sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach servos
  lane1Servo.attach(9);
  lane2Servo.attach(10);

  // Initial state
  setTrafficLights(false); // Green for both lanes
  lane1Servo.write(barrierClosed);
  lane2Servo.write(barrierClosed);

  Serial.begin(9600);
}

void loop() {
  int distance = getDistance();

  if (distance < detectionThreshold) {
    // Pedestrian detected
    setTrafficLights(true); // Red for both lanes
    delay(1000); // Wait for cars to stop
    lane1Servo.write(barrierOpen);
    lane2Servo.write(barrierOpen);
    delay(5000); // Time for pedestrians to cross
    lane1Servo.write(barrierClosed);
    lane2Servo.write(barrierClosed);
    delay(1000); // Ensure barriers are down
    setTrafficLights(false); // Green for both lanes
  }

  delay(100); // Small delay for stability
}

// Function to set traffic lights
void setTrafficLights(bool pedestrianCrossing) {
  if (pedestrianCrossing) {
    // Red for both lanes
    digitalWrite(lane1Red, HIGH);
    digitalWrite(lane1Green, LOW);
    digitalWrite(lane2Red, HIGH);
    digitalWrite(lane2Green, LOW);
  } else {
    // Green for both lanes
    digitalWrite(lane1Red, LOW);
    digitalWrite(lane1Green, HIGH);
    digitalWrite(lane2Red, LOW);
    digitalWrite(lane2Green, HIGH);
  }
}

// Function to get distance from ultrasonic sensor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

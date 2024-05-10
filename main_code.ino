// Define pins for left sensors
const int trigPinLeft1 = 9;
const int echoPinLeft1 = 10;
const int trigPinLeft2 = 11;
const int echoPinLeft2 = 12;

// Define pins for right sensors
const int trigPinRight1 = 2;
const int echoPinRight1 = 3;
const int trigPinRight2 = 4; // Changed from 6 to avoid overlap
const int echoPinRight2 = 5; // Changed from 7 to avoid overlap

// Define pins for motors
const int motorLeft1Pin = 6; // Changed from 2 to avoid overlap
const int motorLeft2Pin = 7; // Changed from 3 to avoid overlap
const int motorRight1Pin = 8; // Changed from 4 to avoid overlap
const int motorRight2Pin = 13; // Changed from 5 to avoid overlap

// Define vibration intensity limits
const int minIntensity = 50; // Minimum vibration intensity
const int maxIntensity = 255; // Maximum vibration intensity

// Variables for left sensors
long durationLeft1;
float distance_cmLeft1;
long durationLeft2;
float distance_cmLeft2;

// Variables for right sensors
long durationRight1;
float distance_cmRight1;
long durationRight2;
float distance_cmRight2;

// Delay between scans/waves when an object is far
int longDelay = 2000; // Long delay
int shortDelay = 500; // Short delay

void setup() {
  // Set up sensor pins
  pinMode(trigPinLeft1, OUTPUT);
  pinMode(echoPinLeft1, INPUT);
  pinMode(trigPinLeft2, OUTPUT);
  pinMode(echoPinLeft2, INPUT);
  pinMode(trigPinRight1, OUTPUT);
  pinMode(echoPinRight1, INPUT);
  pinMode(trigPinRight2, OUTPUT);
  pinMode(echoPinRight2, INPUT);

  // Set up motor pins
  pinMode(motorLeft1Pin, OUTPUT);
  pinMode(motorLeft2Pin, OUTPUT);
  pinMode(motorRight1Pin, OUTPUT);
  pinMode(motorRight2Pin, OUTPUT);

  Serial.begin(9600); // Start serial communication
}

void loop() {
  // Measure distances from all sensors
  measureDistances();

  // Control motors based on proximity readings
  controlMotors();

  // Adjust delay between measurements based on distance readings
  adjustDelay();

  // Delay for stability
  delay(longDelay); // Adjust as necessary
}

void measureDistances() {
  // Measure distance from left sensors
  measureDistance(trigPinLeft1, echoPinLeft1, durationLeft1, distance_cmLeft1);
  measureDistance(trigPinLeft2, echoPinLeft2, durationLeft2, distance_cmLeft2);

  // Measure distance from right sensors
  measureDistance(trigPinRight1, echoPinRight1, durationRight1, distance_cmRight1);
  measureDistance(trigPinRight2, echoPinRight2, durationRight2, distance_cmRight2);
}

void controlMotors() {
  // Calculate vibration intensity for each motor
  int left1Intensity = calculateIntensity(distance_cmLeft1);
  int left2Intensity = calculateIntensity(distance_cmLeft2);
  int right1Intensity = calculateIntensity(distance_cmRight1);
  int right2Intensity = calculateIntensity(distance_cmRight2);

  // Control left motors
  analogWrite(motorLeft1Pin, left1Intensity);
  analogWrite(motorLeft2Pin, left2Intensity);

  // Control right motors
  analogWrite(motorRight1Pin, right1Intensity);
  analogWrite(motorRight2Pin, right2Intensity);
}

int calculateIntensity(float dist) {
  if (dist < 150) { // If distance is less than 1.5 meters
    // Map distance to vibration intensity (linear mapping)
    int intensity = map(dist, 0, 150, maxIntensity, minIntensity);
    return intensity;
  } else {
    return 0; // No vibration if distance is greater than 1.5 meters
  }
}

void measureDistance(int trigPin, int echoPin, long &duration, float &distance_cm) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30ms
  if (duration == 0) {
    // Handle timeout or other errors
    distance_cm = -1; 
  } else {
    distance_cm = duration * 0.0343 / 2;
  }
}

void adjustDelay() {
  if (distance_cmLeft1 < 150 || distance_cmLeft2 < 150 || distance_cmRight1 < 150 || distance_cmRight2 < 150) {
    // If any sensor detects an object within 1.5 meters, set a short delay
    longDelay = shortDelay; 
  } else {
    // If no object is detected within 1.5 meters by any sensor, set a longer delay
    longDelay = 2000; 
  }
}
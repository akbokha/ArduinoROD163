// author: Abdel K. Bokharouss - October 2017
#include <Servo.h> // servo library

// PWMs used by the DC Motors
int pwm1 = 3;
int pwm2 = 5;
int pwm3 = 6;
int pwm4 = 9;

// PWMs used by continuous servos
int pwm5 = 10;
int pwm6 = 11;

int ledPin = 13;

int extendArmValue = 19;
int returnArmValue = 23;
int lowerPlateValue = 8;
int risePlateValue = 11;

// pins on Arduino that will control servo through signals
int gimbalPinX = A0;
int gimbalPinY = A1;

Servo gimbalServoX, gimbalServoY; // servo objects
Servo gearServo, pulleyServo; // servo for the gear & servo to pull up the boarding plate

// initial gimball servo positions
const int GimbalInitPosX = 90;
const int GimbalInitPosY = 90;
const int GimbalStepX = 20;
const int GimbalStepY = 20;
int gimbalPosX = 90;
int gimbalPosY = 90;

// dc motor speed and (de)acceleration
int maxSpeed = 120; // the max speed we want to be able to drive
int motorSpeedLeft; // speed left motor
int motorSpeedRight; // speed right motor
const int motorSpeedStep = 20; // (de)acceleration

// gear servo stop value and step value
int gearStop = 95;
int gearLeft = 90;
int gearRight = 100;
int gearDelay = 1000; // in ms

// gear servo stop value and step value
int pulleyStop = 95; //tbd
int pulleyLeft = 90; //tbd
int pulleyRight = 100; // tbd
int pulleyDelay = 1000; // in ms ; tbd

void outputsToZero() {
  motorSpeedLeft = 0;
  motorSpeedRight = 0;
  analogWrite(pwm1, motorSpeedLeft);
  analogWrite(pwm2, motorSpeedLeft);
  analogWrite(pwm3, motorSpeedRight);
  analogWrite(pwm4, motorSpeedRight);
}

void fullForward() {
  motorSpeedLeft = maxSpeed;
  motorSpeedRight = maxSpeed;
  analogWrite(pwm1, motorSpeedLeft);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, motorSpeedRight);
  analogWrite(pwm4, 0);
}

void fullBackward() {
  motorSpeedLeft = maxSpeed;
  motorSpeedRight = maxSpeed;
  analogWrite(pwm1, 0);
  analogWrite(pwm2, motorSpeedLeft);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, motorSpeedRight);
}

void right() {
  motorSpeedLeft = 0;
  motorSpeedRight = maxSpeed;
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, motorSpeedRight);
  analogWrite(pwm4, 0);
}

void left() {
  motorSpeedRight = 0;
  motorSpeedLeft = maxSpeed;
  analogWrite(pwm1, motorSpeedLeft);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void accelerateForward() {
  motorSpeedRight += motorSpeedStep;
  motorSpeedLeft += motorSpeedStep;
  analogWrite(pwm1, motorSpeedLeft);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, motorSpeedRight);
  analogWrite(pwm4, 0);
}

void accelerateBackward() {
  motorSpeedRight -= motorSpeedStep;
  motorSpeedLeft -= motorSpeedStep;
  analogWrite(pwm1, 0);
  analogWrite(pwm2, motorSpeedLeft);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, motorSpeedRight);
}

void adjustGimbal(int stepsize, bool isGimbal_Y) {
  if (isGimbal_Y) {
    gimbalPosY += stepsize;
    gimbalServoY.write(gimbalPosY);
  } else {
    gimbalPosX += stepsize;
    gimbalServoX.write(gimbalPosX);
  }
}

void gimbalStandardPosition() {
  gimbalPosX = GimbalInitPosX;
  gimbalServoX.write(GimbalInitPosX);
  gimbalPosY = GimbalInitPosY;
  gimbalServoY.write(GimbalInitPosY);
}

void stopGearServo() {
  gearServo.write(gearStop);
}

void turnLeftGearServo() {
  gearServo.write(gearLeft);
  delay(gearDelay);
  gearServo.write(gearStop);
}

void turnRightGearServo() {
  gearServo.write(gearRight);
  delay(gearDelay);
  gearServo.write(gearStop);
}

void stopPulleyServo() {
  pulleyServo.write(pulleyStop);
}

void turnRightPulleyServo() {
  pulleyServo.write(pulleyRight);
  delay(pulleyDelay);
  pulleyServo.write(pulleyStop);
}

void turnLeftPulleyServo() {
  pulleyServo.write(pulleyLeft);
  delay(pulleyDelay);
  pulleyServo.write(pulleyStop);
}

void extendArm() {
  gearServo.write(gearLeft);
  delay(extendArmValue * gearDelay);
  gearServo.write(gearStop);
}

void returnArm() {
  gearServo.write(gearRight);
  delay(returnArmValue * gearDelay);
  gearServo.write(gearStop);
}

void lowerPlate() {
  pulleyServo.write(pulleyLeft);
  delay(lowerPlateValue * pulleyDelay);
  pulleyServo.write(pulleyStop);
}

void risePlate() {
  pulleyServo.write(pulleyRight);
  delay(risePlateValue * pulleyDelay);
  pulleyServo.write(pulleyStop);
}

void processInputBuffer(char cmd) {
  switch (cmd){
    case '8': // DC Motors full forward
      fullForward();
    break;
    case '2': // DC Motors full backward
      fullBackward();
    break;
    case '4': // DC Motors full backward
      left();
    break;
    case '6': // DC Motors full backward
      right();
    break;
    case '0':  // DC Motors reset outputs to 0
      outputsToZero();
    break;
    case '/': // accelerate forward
      accelerateForward();
    break;
    case '.': // accelerate backwards
      accelerateBackward();
    break;
    case 's': // back to init position
      gimbalStandardPosition();
    break;
    case 'w': // incease posY w/ 5 degrees
      adjustGimbal(- GimbalStepY, true);
    break;
    case 'z': // decrease posY w/ 5 degrees
      adjustGimbal((GimbalStepY), true);
    break;
    case 'a': // incease posX w/ 5 degrees
      adjustGimbal(GimbalStepX, false);
    break;
    case 'd': // decrease posX w/ 5 degrees
      adjustGimbal((- GimbalStepX), false);
    break;
    case 'm': // make gear servo turn right
      turnRightGearServo();
    break;
    case 'b': // make gear servo turn left
      turnLeftGearServo();
    break;
    case 'n': // stop gearServo;
      stopGearServo();
    break;
    case 'l': // make gear servo turn right
      turnRightPulleyServo();
    break;
    case 'j': // make gear servo turn left
      turnLeftPulleyServo();
    break;
    case 'k': // stop gearServo;
      stopPulleyServo();
    break;
    case 'p': // stop gearServo;
      extendArm();
    break;
    case 'o': // stop gearServo;
      returnArm();
    break;
    case 'u': // stop gearServo;
      lowerPlate();
    break;
    case 'i': // stop gearServo;
      risePlate();
    break;
    default: // key is not configured
      Serial.println("command not supported");
    break;
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  // set-up code
  Serial1.begin(115200); // Ethernet / Wi-Fi Communication
  // Serial.begin(9600); // USB comminication

  // clear input buffer
  while (Serial1.available()) {
    Serial1.read();
  }

  // attach the right pin to the right servo object
  gimbalServoX.attach(gimbalPinX);
  gimbalServoY.attach(gimbalPinY);
  gimbalStandardPosition(); // move gimbal to standard position

  //dc motor outputs to zero
  outputsToZero();

  // attach pulley servo
  pulleyServo.attach(pwm5);
  pulleyServo.write(pulleyStop);

  // attach gear servo
  gearServo.attach(pwm6);
  gearServo.write(gearStop);
}


void loop() {
  if (Serial1.available() > 0) {
    char command = Serial1.read();
    processInputBuffer(command);
 }
}

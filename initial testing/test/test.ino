// author: Abdel K. Bokharouss - October 2017
#include <Servo.h> // servo library

int pwm1 = 3;
int pwm2 = 5;
int pwm3 = 6;
int pwm4 = 9;
int pwm5 = 10;
int pwm6 = 11;
int ledPin = 13;

int gearSpeed = 95;


// pins on Arduino that will control servo through signals
int gimbalPinX = A0;
int gimbalPinY = A1;

Servo gimbalServoX, gimbalServoY; // servo objects
Servo gearServo;

// initial gimball servo positions
const int GimbalInitPosX = 90;
const int GimbalInitPosY = 90;
const int GimbalStepX = 20;
const int GimbalStepY = 20;
int gimbalPosX = 90;
int gimbalPosY = 90;

int fullspeed = 120; // max value for DC motor to achieve fullspeed

void outputsToZero() {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void fullForward() {
  analogWrite(pwm1, fullspeed);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, fullspeed);
  analogWrite(pwm4, 0);
}

void fullBackward() {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, fullspeed);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, fullspeed);
}

void right() {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, fullspeed);
  analogWrite(pwm3, fullspeed);
  analogWrite(pwm4, 0);
}

void left() {
  analogWrite(pwm1, fullspeed);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, fullspeed);
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
    case 's': // back to init position
      gimbalStandardPosition();
    case 'w': // incease posY w/ 5 degrees
      adjustGimbal(GimbalStepX, true);
      break;
    case 'z': // decrease posY w/ 5 degrees
      adjustGimbal((-GimbalStepX), true);
      break;
    case 'a': // incease posX w/ 5 degrees
      adjustGimbal(GimbalStepY, false);
      break;
    case 'd': // decrease posX w/ 5 degrees
      adjustGimbal((- GimbalStepY), false);
      break;
    case 'm': // decrease posX w/ 5 degrees
      gearSpeed += 5;
      gearServo.write(gearSpeed);
      break;
    case 'b': // decrease posX w/ 5 degrees
      gearSpeed -= 5;
      gearServo.write(gearSpeed);
      break;
    case 'n': // decrease posX w/ 5 degrees
      gearSpeed = 95;
      gearServo.write(gearSpeed);
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

  // attach the right pin to the right servo object
  gimbalServoX.attach(gimbalPinX);
  gimbalServoY.attach(gimbalPinY);

  gimbalStandardPosition(); // move gimbal to standard position
  
  // clear input buffer
  while (Serial1.available()) {
    Serial1.read();
  }
  //outputs to zero
  outputsToZero();
  gearServo.attach(pwm5);
  gearServo.write(gearSpeed);
} // end setup code


void loop() {
  if (Serial1.available() > 0) {
    char command = Serial1.read();
    processInputBuffer(command);
// } else if (Serial.available() > 0) { // USB
  //  char command = Serial.read();
    //processInputBuffer(command);
 }
} // end loop code



#include <Servo.h>

// declare servos
Servo base;
Servo head;
Servo arms;

int basePos = 90;
int headPos = 90;
int armsPos = 90;

// set default scan directions
bool directionRight = true;
bool directionUp = true;

// set speeds of servos
int baseSpeed = 100;
int headSpeed = 80;
int centerSpeed = 100;




//  setup
void setup() {
  //  attach servos to pins
  base.attach(2);
  head.attach(3);
  arms.attach(4);

  Serial.begin(9600);
}// end setup





//  main loop
void loop() {
  //  finder represents a region
  //  where the AprilTag is located
  //  OpenMV sends an integer that is set to finder
  //   __________________
  //  |      |    |      |
  //  |  3   | 2  |  1   |
  //  |______|____|______|
  //  |  4   | 0  |  8   |
  //  |______|____|______|
  //  |      |    |      |
  //  |  5   | 6  |  7   |
  //  |______|____|______|
  //
  //  the servos move the arm to center the AprilTag in the view
  //
  int finder;
  if(Serial.available()) {
    finder = Serial.parseInt();
    arms.write(50);
    centerArm(finder);
  }
  else {
  //  when no apriltags are visible, the robot
  //  defaults to the scan functions
  arms.write(90);
  scanBase();
  scanHead();
  }
}// end main loop

void scanBase() {
  if(base.read() >= 130) {
    directionRight = false;
  }
  if(base.read() <= 50) {
    directionRight = true;
  }
  if(directionRight == true) {
    base.write(base.read() + 1);
    delay(baseSpeed);
    return;
  }
  else {
    base.write(base.read() - 1);
    delay(baseSpeed);
    return;
  }
}

void scanHead() {
  if(head.read() >= 120) {
    directionUp = false;
  }
  if(head.read() <= 110) {
    directionUp = true;
  }
  if(directionUp == true) {
    head.write(head.read() + 1);
    delay(headSpeed);
    return;
  }
  else {
    head.write(head.read() - 1);
    delay(headSpeed);
    return;
  }
}


//  the center() function moves the robot to
//  orient itself so it is looking at the apriltag

void centerArm(int finderIndex) {
  if(finderIndex == 0) {
    base.write(base.read());
    head.write(head.read());
    delay(centerSpeed);
    return;
  }
  if(finderIndex == 1) {
    base.write(base.read() - 1);
    head.write(head.read() - 1);
    delay(centerSpeed);
    return;
  }
  if(finderIndex == 2) {
    base.write(base.read());
    head.write(head.read() - 1);
    delay(centerSpeed);
    return;
  }
  if(finderIndex == 3) {
    base.write(base.read() + 1);
    head.write(head.read() - 1);
    delay(centerSpeed);
    return;
  }
  if(finderIndex == 4) {
    base.write(base.read() + 1);
    head.write(head.read());
    delay(centerSpeed);
    return;
  }
  if(finderIndex == 5) {
    base.write(base.read() + 1);
    head.write(head.read() + 1);
    delay(centerSpeed);
    return;
  }
  if(finderIndex == 6) {
    base.write(base.read());
    head.write(head.read() + 1);
    delay(centerSpeed);
    return;
  }
  if(finderIndex == 7) {
    base.write(base.read() - 1);
    head.write(head.read() + 1);
    delay(centerSpeed);
    return;
  }
  if(finderIndex == 8) {
    base.write(base.read() - 1);
    head.write(head.read());
    delay(centerSpeed);
    return;
  }
}

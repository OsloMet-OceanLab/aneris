#include <Servo.h>

Servo myservo; 
Servo myservo2;

int pos = 0;

void setup() {
  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(11);
  myservo2.attach(12);
  // Servo is stationary.
  myservo.writeMicroseconds(1500);
  myservo2.writeMicroseconds(1500);
}

void loop() {
  // Servo spins forward at full speed for 1 second.
  myservo.writeMicroseconds(1000);
  myservo2.writeMicroseconds(1550);
  delay(1615);
  // Servo is stationary for 1 second.
  myservo.writeMicroseconds(1500);
  myservo2.writeMicroseconds(1500);
  delay(1615);
  // Servo spins in reverse at full speed for 1 second.
  myservo.writeMicroseconds(1550);
  myservo2.writeMicroseconds(1000);
  delay(1615);
  // Servo is stationary for 1 second.
  myservo.writeMicroseconds(1500);
  myservo2.writeMicroseconds(1500);
  delay(1615);
}

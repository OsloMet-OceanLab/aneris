#include <Servo.h>

Servo myservo; 
Servo myservo2;

int pos = 0;

void setup() {
  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(11);
  myservo2.attach(12);
  // Servo is stationary.
  myservo.write(90);
  myservo2.write(90);
}

void loop() {
  // Servo spins forward at full speed for 1 second.
  myservo.write(180);
  myservo2.write(0);
  delay(1000);
  // Servo is stationary for 1 second.
  myservo.write(90);
  myservo2.write(90);
  delay(1000);
  // Servo spins in reverse at full speed for 1 second.
  myservo.write(0);
  myservo2.write(180);
  delay(1000);
  // Servo is stationary for 1 second.
  myservo.write(90);
  myservo2.write(90);
  delay(1000);
}

#include <Servo.h>

Servo servo[2];

#define HALL_1 A7 // has led
#define HALL_2 A6 // does not have led
#define SERVO_1 11
#define SERVO_2 12

void setup() {
  Serial.begin(9600);
  // The servo control wires are connected to Arduino D11-D12 pins.
  servo[0].attach(SERVO_1);
  servo[1].attach(SERVO_2);
  
  // Servos are stationary.
  servo[0].write(90);
  servo[1].write(90);

  // Enabling the digital pins on the hall sensor
  pinMode(HALL_1, INPUT);
  pinMode(HALL_2, INPUT);
}

void loop() {
  servo[0].write(0);
  servo[1].write(180);

  while(analogRead(HALL_1) > 100);

  servo[0].write(90);
  servo[1].write(90);

  delay(1000);

  servo[0].write(180);
  servo[1].write(0);

  int hall_2_read = analogRead(HALL_2);
  while(250 < hall_2_read && hall_2_read < 900) hall_2_read = analogRead(HALL_2);
  
  servo[0].write(90);
  servo[1].write(90);

  delay(1000);
}

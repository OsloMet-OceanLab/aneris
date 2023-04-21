#include <Servo.h>

Servo myservo; 
Servo myservo2;

int hallSensorPin_1 = 2;
int hallSensorPin_2 = 3;
int hallSensorValue_1 = 0;
int hallSensorValue_2 = 0;

void setup() {
  Serial.begin(9600);

  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(11);
  myservo2.attach(12);
  // Servo is stationary.
  myservo.write(90);
  myservo2.write(90);

  // Enabling the digital pins on the hall sensor
  pinMode(hallSensorPin_1, INPUT);
  pinMode(hallSensorPin_2, INPUT);
}

void loop() {
  Serial.print("hallSensorValue_1: ");
  Serial.println(hallSensorValue_1);
  Serial.print("hallSensorValue_2: ");
  Serial.println(hallSensorValue_2);

  // Servo spins forward at full speed.
  myservo.write(0);
  myservo2.write(180);
  Serial.println("Spinning forward!");
 
  while(!digitalRead(hallSensorPin_1));
  Serial.println("Stopped!");
  myservo.write(90);
  myservo2.write(90);

  delay(1000);

  // Servo spins forward at full speed.
  myservo.write(180);
  myservo2.write(0);
  Serial.println("Spinning backwards!");
  
  while(!digitalRead(hallSensorPin_2));
  Serial.println("Stopped!");
  myservo.write(90);
  myservo2.write(90);

  delay(1000);
}





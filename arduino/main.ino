#include <Stepper.h>

#define stepsPerRevolution 200 // change this to fit the number of steps per revolution for your motor

// initialize the stepper library on pins 9 through 12:
Stepper stepper(stepsPerRevolution, 9, 10 , 11, 12);

void setup()
{
    // set the speed at 100 rpm:
    stepper.setSpeed(100);

    // initialize the serial port:
    Serial.begin(9600);

    // initialize arduino read pin
    pinMode(5, INPUT);

    // initialize relay pins
    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
}

void loop()
{
    if(digitalRead(5))
    {
        // enable motors and drivers
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);

        delay(1000); // 1s delay, in case the drivers need a second to boot properly

        stepper.step(800);
        delay(100);
        stepper.step(-1600);
        delay(100);
        stepper.step(800);
        delay(100);

        // disable motors and drivers
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
    }
}

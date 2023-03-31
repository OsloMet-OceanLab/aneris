#include <Stepper.h>

#define STEPS_PER_REVOLUTION 200 // change this to fit the number of steps per revolution for your motor

// gpio pins
#define REL_1 6
#define REL_2 7
#define PI_1  8

// motor pins
#define M_1 9
#define M_2 10
#define M_3 11
#define M_4 12

// initialize the stepper library on pins 9 through 12:
Stepper stepper(STEPS_PER_REVOLUTION, M_1, M_2, M_3, M_4);

void setup()
{
    // set the speed at 100 rpm:
    stepper.setSpeed(100);

    // initialize the serial port:
    Serial.begin(9600);

    // initialize arduino read pin
    pinMode(PI_1, INPUT);

    // initialize relay pins
    pinMode(REL_1, OUTPUT);
    digitalWrite(REL_1, LOW);
    pinMode(REL_2, OUTPUT);
    digitalWrite(REL_2, LOW);
}

void loop()
{
    if(digitalRead(5) || 1)
    {
        // enable motors and drivers
        digitalWrite(REL_1, HIGH);
        digitalWrite(REL_2, HIGH);

        delay(1000); // 1s delay, in case the drivers need a second to boot properly

        stepper.step(1600);
        delay(100);
        stepper.step(-1600);
        delay(100);

        // disable motors and drivers
        digitalWrite(REL_1, LOW);
        digitalWrite(REL_2, LOW);
    }
}

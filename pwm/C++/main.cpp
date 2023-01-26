#include <iostream>
#include "PWM.hpp"
#include <wiringPi.h>

int main(void)
{
	wiringPiSetupGpio();
	pinMode(4, OUTPUT);
	while (true)
	{
		digitalWrite(4, HIGH);
		delay(1000);
		digitalWrite(4, LOW);
		delay(1000);
		break;
	}
	return 0;
}

/**
 * test network connection to computer (ping?)
 * check both network cards are up (hydrophone and land)
 * if the above works, begin by opening port for listening to commands
 * if it does NOT work, reboot once and repeat
 * if on 3rd attempt still does not work, log and turn off
 * will have 2 ports: ex. 5000 to send video stream, 5001 to listen to commands
 * when command is received, needs to return successfulness
 * program needs to, at a minimum, recognize commands for reboot/shutdown
 * can probably use switch statement with integer for command
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "../gpio/C++/GPIO.h"
#include "Log.h"

int main(void)
{
	// verify user is root
	if(geteuid())
	{
		Logger::log("This program needs to be run as root", Logger::LOG_FATAL);
		printf("reboot\n"); // system("reboot");
		return 0;
	}
	Logger::log("Verified user permission", Logger::LOG_INFO);
	
	// verify gpio is available
	try
	{
		gpio::GPIO *test_gpio = new gpio::GPIO(17);
		test_gpio->setdir(gpio::GPIO_INPUT);
		if(!test_gpio->getval()) throw gpio::GPIOError("GPIO unavailable");
		delete test_gpio;
	}
	catch(gpio::GPIOError& e)
	{
		Logger::log(e.what(), Logger::LOG_FATAL);
		printf("reboot\n"); // system("reboot");
		return 0;
	}
	Logger::log("Verified GPIO is available", Logger::LOG_INFO);
	
	// test uplink to hydrophone/fathom tether interface
	//blah
	
	uint8_t command = 2;
	while (true)
	{
		// logic to receive command
		
		switch(command)
		{
			case 0: // shutdown
				Logger::log("Received shutdown command", Logger::LOG_INFO);
				system("poweroff");
				break;
			case 1:
				Logger::log("Received reboot command", Logger::LOG_INFO);
				system("reboot");
				break;
			default: // return that command is invalid
				Logger::log("Receiven an invalid command", Logger::LOG_INFO)
				return 0;
		}
	}
}

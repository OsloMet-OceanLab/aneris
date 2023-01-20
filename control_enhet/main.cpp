/**
 * test network connection to computer (ping?)
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

int main(void)
{
	// check user is root
	if(geteuid())
	{
		fputs("This program needs to be run as root\n", stderr);
		printf("reboot");//system("reboot");
	}
	
	// check if connected to network
	//blah
	
	// check if gpio is available
	//use gpio.h and read pin, if no fails all good
	GPIO test_gpio(17);
	test_gpio.setdir("in");
	if(!test_gpio.getval())
	{
		fputs("GPIO unavailable", stderr);
		printf("reboot");//system("reboot");
	}
	
	unsigned int command = 0;
	while (true)
	{
		// logic to receive command
		
		switch(command)
		{
			case 0: // shutdown
				system("poweroff");
				break;
			case 1:
				system("reboot");
				break;
			default:
				// return that command is invalid
				return 0;
		}
	}
	//return 0;
}

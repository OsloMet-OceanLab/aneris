/**
 * check hydrophone is up (led pin on connecor might be a good play)
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
#include <csignal>
#include <pthread.h>
#include "../gpio/C++/GPIO.hpp"
#include "Logger/Logger.hpp"

void handler(const int signum);

int main(void)
{
	// register signal handler
	signal(SIGINT, handler);
	signal(SIGTERM, handler);
	signal(SIGABRT, handler);
	signal(SIGSEGV, handler);
	signal(SIGFPE, handler);
	signal(SIGBUS, handler);
	
	// verify user is root
	if(geteuid())
	{
		Logger::log(Logger::LOG_FATAL, "This program needs to be run as root");
		printf("reboot\n"); // system("reboot");
		return 0;
	}
	Logger::log(Logger::LOG_INFO, "Verified user permission");
	
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
		Logger::log(Logger::LOG_FATAL, e.what());
		printf("reboot\n"); // system("reboot");
		return 0;
	}
	Logger::log(Logger::LOG_INFO, "Verified GPIO is available");
	
	// test uplink to fathom tether interface
	// if main device uses e.g. ip 192.168.2.1, then
	int counter = 0, tether_up;
	std::string attempt = "";
	do
	{
		attempt = "Testing connection to land, attempt " + std::to_string(counter + 1);
		Logger::log(Logger::LOG_INFO, attempt);
		tether_up = system("ping -c 1 192.168.2.1"); // 0 on successful ping, other integer otherwise

		if(tether_up && counter < 2)
		{
			++counter;
			sleep(1);
		}
		else break;
	} while(true);
	
	if(tether_up) Logger::log(Logger::LOG_WARNING, "No connection to land");
	else Logger::log(Logger::LOG_INFO, "Verified connection to land is available");
	
	uint8_t command = 2;
	while (true)
	{
		// logic to receive command
		
		switch(command)
		{
			case 0: // shutdown
				Logger::log(Logger::LOG_INFO, "Received shutdown command");
				system("poweroff");
				break;
			case 1:
				Logger::log(Logger::LOG_INFO, "Received reboot command");
				system("reboot");
				break;
			default: // return that command is invalid
				Logger::log(Logger::LOG_INFO, "Receiven an invalid command");
				return 0;
		}
	}
}

void handler(const int signum)
{
	exit(signum);
}


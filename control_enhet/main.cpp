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

#define COMMAND_PORT 5000
#define LOG_PORT 5001
#define STREAMING_PORT 5002

#define GPIO_LIGHTS 4
#define GPIO_TEST 17
#define GPIO_HYDROPHONE 19
#define GPIO_WIPER 21

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
	if(geteuid()) Logger::log(Logger::LOG_ERROR, "This program needs to be run as root");
	else Logger::log(Logger::LOG_INFO, "Verified user permission");
	
	// verify gpio is available
	try
	{
		gpio::GPIO *test_gpio = new gpio::GPIO(GPIO_TEST);
		if(!test_gpio) throw gpio::GPIOError("Couldn't allocate memory for 'test_gpio' variable");
		test_gpio->setdir(gpio::GPIO_INPUT);
		if(test_gpio->getval()) Logger::log(Logger::LOG_INFO, "Verified GPIO is available");
		else Logger::log(Logger::LOG_ERROR, "GPIO unavailable");
		delete test_gpio;
	}
	catch(gpio::GPIOError& e)
	{
		Logger::log(Logger::LOG_ERROR, e.what());
		Logger::log(Logger::LOG_ERROR, "GPIO unavailable");
	}
	
	
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
	
	// test that hydrophone is up
	//assuming that the led pin on the connector is on when it has power:
	try
	{
		gpio::GPIO *hydrophone = new gpio::GPIO(GPIO_HYDROPHONE);
		if(!hydrophone) throw gpio::GPIOError("Couldn't allocate memory for 'hydrophone' variable");
		hydrophone->setdir(gpio::GPIO_INPUT);
		if(hydrophone->getval()) Logger::log(Logger::LOG_INFO, "Verified Hydrophone is available");
		else Logger::log(Logger::LOG_ERROR, "Hydrophone is not available");
		delete hydrophone;
	}
	catch(gpio::GPIOError& e)
	{
		Logger::log(Logger::LOG_ERROR, e.what());
		Logger::log(Logger::LOG_ERROR, "Hydrophone is not available");
	}
	
	// start command listener
	int server_fd, socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char* msg = "Message from the server";
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0)
	{
		Logger::log(Logger::LOG_FATAL, "Can't create socket");
		return 0; //system("reboot")
	}
	
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		Logger::log(Logger::LOG_FATAL, "Can't attach socket to port");
		return 0; //system("reboot")
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(COMMAND_PORT);
	
	int bound = bind(server_fd, (struct sockaddr*)&address, sizeof(address));
	if(bound)
	{
		Logger::log(Logger::LOG_FATAL, "Can't bind to port");
		return 0; //system("reboot")
	}
	
	if(listen(server_fd, 3) < 0)
	{
		Logger::log(Logger::LOG_FATAL, "Can't listen to port");
		return 0; //system("reboot")
	}
	
	// start log web browser
	// start python web browser
	//create new thread for the web servers
	
	uint8_t command = 2;

	while (true)
	{
		// logic to receive command
		memset(buffer, 0, 1024);
		socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		if(socket < 0)
		{
			Logger::log(Logger::LOG_FATAL, "Can't accept new connections");
			return 0; //system("reboot")
		}
		
		valread = read(socket, buffer, 1024);
		printf("%s, %d\n", buffer, valread);
		
		send(socket, msg, strlen(msg), 0);
		printf("Message sent\n");
		
		close(socket);
		
		buffer[valread] = '\0';
		
		switch(command)
		{
			case 0: // shutdown
			{
				Logger::log(Logger::LOG_INFO, "Received shutdown command");
				shutdown(server_fd, SHUT_RDWR);
				system("poweroff");
				break;
			}
			case 1: // reboot
			{
				Logger::log(Logger::LOG_INFO, "Received reboot command");
				system("reboot");
				break;
			}
			case 2: // turn lights on/off
			{
				gpio::GPIO *lights = new gpio::GPIO(GPIO_LIGHTS);
				try
				{
					if(!lights) throw gpio::GPIOError("Couldn't allocate memory for 'lights' variable");
					lights->setdir(gpio::GPIO_OUTPUT);
					if(lights->getval())
					{
						lights->setval(gpio::GPIO_LOW);
						Logger::log(Logger::LOG_INFO, "Disabled lights");
					}
					else
					{
						lights->setval(gpio::GPIO_HIGH);
						Logger::log(Logger::LOG_INFO, "Enabled lights");
					}
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
				}
				delete lights;
				break;
			}
			case 3: // turn wipers on/off
			{
				gpio::GPIO *wiper = new gpio::GPIO(GPIO_WIPER);
				try
				{
					if(!wiper) throw gpio::GPIOError("Couldn't allocate memory for 'wiper' variable");
					wiper->setdir(gpio::GPIO_OUTPUT);
					if(wiper->getval())
					{
						wiper->setval(gpio::GPIO_LOW);
						Logger::log(Logger::LOG_INFO, "Disabled wiper");
					}
					else
					{
						wiper->setval(gpio::GPIO_HIGH);
						Logger::log(Logger::LOG_INFO, "Enabled wiper");
					}
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
				}
				delete wiper;
				break;
			}
			case 4: {}
			case 5: {}
			case 6: {}
			default: // return that command is invalid
			{
				Logger::log(Logger::LOG_INFO, "Receiven an invalid command");
				return 0;
			}
		}
	}
}

void handler(const int signum)
{
	exit(signum);
}

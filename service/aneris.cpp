#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <csignal>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "gpio/GPIO.hpp"
#include "Logger/Logger.hpp"

#define WEB_SERVER_PORT 5000
#define COMMAND_PORT 5001

#define GPIO_LIGHTS 4
#define GPIO_TEST 17
#define GPIO_HYDROPHONE 19
#define GPIO_WIPER 21

#define COMMAND_BUFFER_SIZE 4

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
		gpio::GPIO *test_gpio = new gpio::GPIO(GPIO_TEST, gpio::GPIO_INPUT);
		if(!test_gpio) throw gpio::GPIOError("Couldn't allocate memory for 'test_gpio' variable");
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
	
	if(tether_up) Logger::log(Logger::LOG_WARN, "No connection to land");
	else Logger::log(Logger::LOG_INFO, "Verified connection to land is available");
	
	// test that hydrophone is up
	//assuming that the led pin on the connector is on when it has power:
	try
	{
		gpio::GPIO *hydrophone = new gpio::GPIO(GPIO_HYDROPHONE, gpio::GPIO_INPUT);
		if(!hydrophone) throw gpio::GPIOError("Couldn't allocate memory for 'hydrophone' variable");
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
	struct sockaddr_in address;
	int server_fd, sock, valread, opt = 1, addrlen = sizeof(address);
	char buffer[COMMAND_BUFFER_SIZE] = { 0 };
	
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
	std::string msg = "";

	while (true)
	{
		// logic to receive command
		msg = "";
		memset(buffer, 0, COMMAND_BUFFER_SIZE);
		sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		if(socket < 0)
		{
			Logger::log(Logger::LOG_FATAL, "Can't accept new connections");
			return 0; //system("reboot")
		}
		
		valread = read(sock, buffer, COMMAND_BUFFER_SIZE);
		buffer[valread] = '\0';
		command = atoi(buffer);
		
		switch(command)
		{
			case 0: // shutdown
			{
				msg = "Received shutdown command";
				Logger::log(Logger::LOG_INFO, msg);
				send(sock, msg.c_str(), strlen(msg.c_str()), 0);
				
				close(sock);
				shutdown(server_fd, SHUT_RDWR);
				system("poweroff");
				break;
			}
			case 1: // reboot
			{
				msg = "Received reboot command";
				Logger::log(Logger::LOG_INFO, msg);
				send(sock, msg.c_str(), strlen(msg.c_str()), 0);

				close(sock);
				shutdown(server_fd, SHUT_RDWR);
				system("reboot");
				break;
			}
			case 2: // turn lights on/off
			{
				try
				{
					gpio::GPIO *lights = new gpio::GPIO(GPIO_LIGHTS, gpio::GPIO_OUTPUT);
					if(!lights) throw gpio::GPIOError("Couldn't allocate memory for 'lights' variable");
					if(lights->getval())
					{
						msg = "Disabled lights";
						lights->setval(gpio::GPIO_LOW);
						Logger::log(Logger::LOG_INFO, msg);
					}
					else
					{
						msg = "Enabled lights";
						lights->setval(gpio::GPIO_HIGH);
						Logger::log(Logger::LOG_INFO, msg);
					}
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
					msg = std::string(e.what());
				}
				delete lights;
				send(sock, msg.c_str(), strlen(msg.c_str()), 0);
				break;
			}
			case 3: // turn wipers on/off
			{
				try
				{
					gpio::GPIO *wiper = new gpio::GPIO(GPIO_WIPER, gpio::GPIO_OUTPUT);
					if(!wiper) throw gpio::GPIOError("Couldn't allocate memory for 'wiper' variable");
					if(wiper->getval())
					{
						msg = "Disabled wiper";
						wiper->setval(gpio::GPIO_LOW);
						Logger::log(Logger::LOG_INFO, msg);
					}
					else
					{
						msg = "Enabled wiper";
						wiper->setval(gpio::GPIO_HIGH);
						Logger::log(Logger::LOG_INFO, msg);
					}
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
					msg = std::string(e.what());
				}
				delete wiper;
				send(sock, msg.c_str(), strlen(msg.c_str()), 0);
				break;
			}
			case 4: {} // clean up log file
			case 5: {}
			case 6: {}
			default: // return that command is invalid
			{
				Logger::log(Logger::LOG_INFO, "Receiven an invalid command");
				return 0;
			}
		}
		close(sock);
	}
}

void handler(const int signum)
{
	exit(signum);
}

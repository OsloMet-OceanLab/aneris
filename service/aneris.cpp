#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <csignal>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>

#include "gpio/GPIO.hpp"
#include "Logger/Logger.hpp"

#define WEB_SERVER_PORT 5000

#define SOCKET_PATH "/var/run/aneris.socket"
#define COMMAND_SIZE 4

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
	int command = 2;
	int sock, len;
	struct sockaddr_un addr, peer_sock;
	char buf[COMMAND_SIZE];
	memset(&sock, 0, sizeof(struct sockaddr_un));
	memset(buf, 0, COMMAND_SIZE);
	
	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		Logger::log(Logger::LOG_FATAL, "Couldn't open socket");
		return 0;
	} else Logger::log(Logger::LOG_INFO, "open socket");
	
	strcpy(addr.sun_path, SOCKET_PATH);
	addr.sun_family = AF_UNIX;
	
	len = sizeof(addr);
	
	if(bind(sock, (struct sockaddr*) &addr, len))
	{
		Logger::log(Logger::LOG_FATAL, "Couldn't bind to socket");
		goto end;
	} else Logger::log(Logger::LOG_INFO, "bound socket");
	
	// start log web browser
	// start python web browser
	//create new thread for the web servers

	while (true)
	{
		// logic to receive command
		Logger::log(Logger::LOG_INFO, "waiting socket");
		if(recvfrom(sock, buf, COMMAND_SIZE, 0, (struct sockaddr *) &peer_sock, (socklen_t*) &len) < 0)
		{
			Logger::log(Logger::LOG_FATAL, "Couldn't receive data");
			goto end;
		}
		command = atoi(buf);
		
		switch(command)
		{
			case 0: // shutdown
			{
				Logger::log(Logger::LOG_INFO, "Received shutdown command");

				// temporary
				goto end;

				system("poweroff");
				break;
			}
			case 1: // reboot
			{
				Logger::log(Logger::LOG_INFO, "Received reboot command");

				// temporary
				goto end;
				
				system("reboot");
				break;
			}
			case 2: // turn lights on/off
			{
				gpio::GPIO *lights = nullptr;
				try
				{
					lights = new gpio::GPIO(GPIO_LIGHTS, gpio::GPIO_OUTPUT);
					if(!lights) throw gpio::GPIOError("Couldn't allocate memory for 'lights' variable");
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
				gpio::GPIO *wiper = nullptr;
				try
				{
					wiper = new gpio::GPIO(GPIO_WIPER, gpio::GPIO_OUTPUT);
					if(!wiper) throw gpio::GPIOError("Couldn't allocate memory for 'wiper' variable");
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
			case 4: // clean up log file
			{
				Logger::clearLog();
				break;
			}
			case 5: {}
			case 6: {}
			default: // return that command is invalid
			{
				Logger::log(Logger::LOG_INFO, "Received an invalid command");
end:
				close(sock);
				return 0;
			}
		}
	}
}

void handler(const int signum)
{
	exit(signum);
}

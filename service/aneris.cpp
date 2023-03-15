/************************************/
/* aneris project                   */
/* insert information on license    */
/* and authors here                 */
/*                                  */
/* brief description of the program */
/* as well                          */
/************************************/

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#include "gpio/GPIO.hpp"
#include "Logger/Logger.hpp"
#include "Web_Server/Web_Server.hpp"

#define WEB_SERVER_PORT 80

#define SOCKET_PATH "/var/run/aneris.sock"
#define COMMAND_SIZE 2

#define GPIO_LIGHTS 4
#define GPIO_TEST 17
#define GPIO_HYDROPHONE 27
#define GPIO_WIPER 22

void sig_handler(int signum);

int main(void)
{
	/***************************/
	/* register signal handler */
	/***************************/
	std::signal(SIGINT, sig_handler);
	std::signal(SIGTERM, sig_handler);
	std::signal(SIGABRT, sig_handler);
	std::signal(SIGSEGV, sig_handler);
	std::signal(SIGFPE, sig_handler);
	std::signal(SIGILL, sig_handler);
	
	/***********************/
	/* verify user is root */
	/***********************/
	if(geteuid())
	{
		Logger::log(Logger::LOG_FATAL, "This program needs to be run as root");
		exit(1);
	} else Logger::log(Logger::LOG_INFO, "Verified user permission");
	
	/****************************/
	/* verify gpio is available */
	/****************************/
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
	
	/******************************************/
	/* test uplink to fathom tether interface */
	/*                                        */
	/* 0 on successful ping, any other        */
	/* integer otherwise                      */
	/******************************************/
	int ping_counter = 0, tether_up = 1;
	std::string attempt = "";
	do
	{
		attempt = "Testing connection to land, attempt " + std::to_string(ping_counter + 1);
		Logger::log(Logger::LOG_INFO, attempt);
		tether_up = system("ping -c 1 10.44.6.51");

		if(tether_up && ping_counter < 2)
		{
			++ping_counter;
			sleep(3);
		} else break;
	} while(true);
	
	if(tether_up) Logger::log(Logger::LOG_WARN, "No connection to land");
	else Logger::log(Logger::LOG_INFO, "Verified connection to land is available");
	
	/******************************************/
	/* test hydrophone is connected           */
	/*                                        */
	/* currently checks that led pin is       */
	/* enabled, but could also check that     */
	/* network card is up? TBD                */
	/* N.B. most likely unnecessary, can      */
	/* probably be deleted                    */
	/******************************************/
	/*try
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
	}*/
	
	/**************************/
	/* start command listener */
	/**************************/
	int sock, len;
	struct sockaddr_un server_sockaddr, peer_sock;
	char buf[COMMAND_SIZE];
	memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));

	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		Logger::log(Logger::LOG_FATAL, "Couldn't create socket");
		exit(2);
	}

	server_sockaddr.sun_family = AF_UNIX;
	memcpy(server_sockaddr.sun_path, SOCKET_PATH, strlen(SOCKET_PATH));
	len = sizeof(server_sockaddr);
	unlink(SOCKET_PATH);

	if (bind(sock, (struct sockaddr*) &server_sockaddr, len) < 0)
	{
		Logger::log(Logger::LOG_FATAL, "Couldn't bind to socket");
		close(sock);
		exit(2);
	}

	/***************************************************/
	/* start python web server                         */
	/*                                                 */
	/* N.B. do NOT modify the 'PYTHONPATH' environment */
	/* variable, it WILL break the program             */
	/***************************************************/
	setenv("PYTHONPATH", ".", 1);
	pthread_t ws_thread;
	bool thread_running;
	int ws_port = WEB_SERVER_PORT;
	if (pthread_create(&ws_thread, NULL, &Web_Server::serve, &ws_port))
	{
		Logger::log(Logger::LOG_FATAL, "Couldn't start web server process");
		close(sock);
		exit(3); //system("reboot");
	}
	Logger::log(Logger::LOG_INFO, "Started web server process");
	thread_running = true;

	/*******************/
	/* begin main loop */
	/*******************/
	Logger::log(Logger::LOG_INFO, "Listening to socket");
	while (true)
	{
		memset(buf, 0, COMMAND_SIZE);
		if (recvfrom(sock, buf, COMMAND_SIZE, 0, (struct sockaddr*) &peer_sock, (socklen_t*) &len) < 0)
		{
			Logger::log(Logger::LOG_FATAL, "Couldn't receive data");
			exit(2);
		}
		
		switch(atoi(buf))
		{
			case 0: // handle received commands that are not numerical and/or atoi() errors
			{
				Logger::log(Logger::LOG_ERROR, "Non numerical command received");
				break;
			}
			case 1: // shutdown
			{
				Logger::log(Logger::LOG_INFO, "Received shutdown command");
				//system("poweroff");
				break;
			}
			case 2: // reboot
			{
				Logger::log(Logger::LOG_INFO, "Received reboot command");
				//system("reboot");
				break;
			}
			case 3: // turn lights on
			{
				gpio::GPIO *lights = nullptr;
				try
				{
					lights = new gpio::GPIO(GPIO_LIGHTS, gpio::GPIO_OUTPUT);
					if(!lights) throw gpio::GPIOError("Couldn't allocate memory for 'lights' variable");
					lights->setval(gpio::GPIO_HIGH);
					Logger::log(Logger::LOG_INFO, "Enabled lights");
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
				}
				delete lights;
				break;
			}
			case 4: // turn lights off
			{
				gpio::GPIO *lights = nullptr;
				try
				{
					lights = new gpio::GPIO(GPIO_LIGHTS, gpio::GPIO_OUTPUT);
					if(!lights) throw gpio::GPIOError("Couldn't allocate memory for 'lights' variable");
					lights->setval(gpio::GPIO_LOW);
					Logger::log(Logger::LOG_INFO, "Disabled lights");
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
				}
				delete lights;
				break;
			}
			case 5: // turn wipers on
			{
				gpio::GPIO *wiper = nullptr;
				try
				{
					wiper = new gpio::GPIO(GPIO_WIPER, gpio::GPIO_OUTPUT);
					if(!wiper) throw gpio::GPIOError("Couldn't allocate memory for 'wiper' variable");
					wiper->setval(gpio::GPIO_HIGH);
					Logger::log(Logger::LOG_INFO, "Enabled wiper");
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
				}
				delete wiper;
				break;
			}
			case 6: // turn wipers off
			{
				gpio::GPIO *wiper = nullptr;
				try
				{
					wiper = new gpio::GPIO(GPIO_WIPER, gpio::GPIO_OUTPUT);
					if(!wiper) throw gpio::GPIOError("Couldn't allocate memory for 'wiper' variable");
					wiper->setval(gpio::GPIO_LOW);
					Logger::log(Logger::LOG_INFO, "Disabled wiper");
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
				}
				delete wiper;
				break;
			}
			case 7: // turn porpoise on
			{
				gpio::GPIO *hydrophone = nullptr;
				try
				{
					hydrophone = new gpio::GPIO(GPIO_HYDROPHONE, gpio::GPIO_OUTPUT);
					if(!hydrophone) throw gpio::GPIOError("Couldn't allocate memory for 'hydrophone' variable");
					hydrophone->setval(gpio::GPIO_HIGH);
					Logger::log(Logger::LOG_INFO, "Enabled hydrophone");
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
				}
				delete hydrophone;
				break;
			}
			case 8: // turn porpoise off
			{
				gpio::GPIO *hydrophone = nullptr;
				try
				{
					hydrophone = new gpio::GPIO(GPIO_HYDROPHONE, gpio::GPIO_OUTPUT);
					if(!hydrophone) throw gpio::GPIOError("Couldn't allocate memory for 'hydrophone' variable");
					hydrophone->setval(gpio::GPIO_LOW);
					Logger::log(Logger::LOG_INFO, "Disabled hydrophone");
				}
				catch(gpio::GPIOError& e)
				{
					Logger::log(Logger::LOG_ERROR, e.what());
				}
				delete hydrophone;
				break;
			}
			case 9: // clean up log file
			{
				Logger::clearLog();
				break;
			}
			case 10: // start web server process
			{
				if(!thread_running)
				{
					if (pthread_create(&ws_thread, NULL, &Web_Server::serve, &ws_port))
					{
						Logger::log(Logger::LOG_FATAL, "Couldn't start web server process");
						exit(3);
					}
					Logger::log(Logger::LOG_INFO, "Started web server process");
				}
				break;
			}
			case 11: // end web server process
			{
				if(thread_running)
				{
					Logger::log(Logger::LOG_INFO, "Ending web server process");
					if (pthread_cancel(ws_thread)) Logger::log(Logger::LOG_ERROR, "Couldn't stop web server process");
					else Logger::log(Logger::LOG_INFO, "Ended web server process");
				}
				break;
			}
			default: // return that command is invalid
			{
				Logger::log(Logger::LOG_INFO, "Received an invalid command");
				close(sock);
				exit(0);
			}
		}
	}
}

void sig_handler(int signum)
{
	switch(signum)
	{
		case 2:
		{
			Logger::log(Logger::LOG_INFO, "Received SIGINT signal");
			break;
		}
		case 3:
		{
			Logger::log(Logger::LOG_INFO, "Received SIGILL signal");
			break;
		}
		case 6:
		{
			Logger::log(Logger::LOG_INFO, "Received SIGABRT signal");
			break;
		}
		case 8:
		{
			Logger::log(Logger::LOG_INFO, "Received SIGFPE signal");
			break;
		}
		case 11:
		{
			Logger::log(Logger::LOG_INFO, "Received SIGSEGV signal");
			break;
		}
		case 15:
		{
			Logger::log(Logger::LOG_INFO, "Received SIGTERM signal");
			break;
		}
		default:
		{
			Logger::log(Logger::LOG_INFO, "Received unknown signal");
			break;
		}
	}

	exit(signum);
}

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
#include <stdlib.h>
#include <csignal>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <Python.h>

#include "gpio/GPIO.hpp"
#include "Logger/Logger.hpp"

#define WEB_SERVER_PORT 5000
#define PY_SSIZE_T_CLEAN

#define SOCKET_PATH "/var/run/aneris.sock"
#define COMMAND_SIZE 2

#define GPIO_LIGHTS 4
#define GPIO_TEST 17
#define GPIO_HYDROPHONE 19
#define GPIO_WIPER 21

void handler(const int signum);
int web_server(const int port);

int main(void)
{
	/***************************/
	/* register signal handler */
	/***************************/
	signal(SIGINT, handler);
	signal(SIGTERM, handler);
	signal(SIGABRT, handler);
	signal(SIGSEGV, handler);
	signal(SIGFPE, handler);
	signal(SIGBUS, handler);
	
	/***********************/
	/* verify user is root */
	/***********************/
	if(geteuid())
	{
		Logger::log(Logger::LOG_FATAL, "This program needs to be run as root");
		//return 0;//system("reboot");
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
	int counter = 0, tether_up = 1;
	std::string attempt = "";
	do
	{
		// this has been temporarily disabled, reenable when deploying to prod
		Logger::log(Logger::LOG_INFO, "Ping disabled");
		break;
		/*
		attempt = "Testing connection to land, attempt " + std::to_string(counter + 1);
		Logger::log(Logger::LOG_INFO, attempt);
		tether_up = system("ping -c 1 192.168.2.1");

		if(tether_up && counter < 2)
		{
			++counter;
			sleep(3);
		} else break;*/
	} while(true);
	
	if(tether_up) Logger::log(Logger::LOG_WARN, "No connection to land");
	else Logger::log(Logger::LOG_INFO, "Verified connection to land is available");
	
	/******************************************/
	/* test hydrophone is connected           */
	/*                                        */
	/* currently checks that led pin is       */
	/* enabled, but could also check that     */
	/* network card is up? TBD                */
	/******************************************/
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
        goto end;
    }

    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCKET_PATH); 
    len = sizeof(server_sockaddr);
    unlink(SOCKET_PATH);

    if (bind(sock, (struct sockaddr*) &server_sockaddr, len) < 0)
    {
        Logger::log(Logger::LOG_FATAL, "Couldn't bind to socket");
        goto end;
    }

	/******************************************/
	/* start python web browser in new thread */
	/******************************************/
	web_server(WEB_SERVER_PORT);
	Logger::log(Logger::LOG_INFO, "Started web server");

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
			goto end;
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

				// temporary
				goto end;

				system("poweroff");
				break;
			}
			case 2: // reboot
			{
				Logger::log(Logger::LOG_INFO, "Received reboot command");

				// temporary
				goto end;
				
				system("reboot");
				break;
			}
			case 3: // turn lights on/off
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
			case 4: // turn wipers on/off
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
			case 5: // clean up log file
			{
				Logger::clearLog();
				break;
			}
			case 6: {}
			case 7: {}
			default: // return that command is invalid
			{
				Logger::log(Logger::LOG_INFO, "Received an invalid command");
end: // temporary
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

int web_server(const int port)
{
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;
	int i;
	
	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault("web_server");
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	if(pModule)
	{
		pFunc = PyObject_GetAttrString(pModule, "start");
		if(pFunc && PyCallable

	return port;

	#define PY_SSIZE_T_CLEAN
#include <Python.h>

int
main(int argc, char *argv[])
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }

    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault(argv[1]);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {
                pValue = PyLong_FromLong(atoi(argv[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}
}

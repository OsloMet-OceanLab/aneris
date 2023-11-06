/******************/
/* aneris project */
/******************/

#ifndef _ANERIS_HPP
#define _ANERIS_HPP

/***********/
/* Imports */
/***********/
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <memory>

#include "gpio/GPIO.hpp"
#include "Logger/Logger.hpp"
#include "Web_Server/Web_Server.hpp"

/*************/
/* Constants */
/*************/
#ifndef WEB_SERVER_PORT
#   define WEB_SERVER_PORT 80
#endif

#define SOCKET_PATH "/var/run/aneris.sock"
#define COMMAND_SIZE 2

#ifndef GPIO_LIGHTS
#define GPIO_LIGHTS 4
#endif
#ifndef GPIO_TEST
#define GPIO_TEST 17
#endif
#ifndef GPIO_HYDROPHONE
#define GPIO_HYDROPHONE 27
#endif
#ifndef GPIO_WIPER
#define GPIO_WIPER 22
#endif

/**************/
/* Prototypes */
/**************/
void turn_lights_on();
void turn_lights_off();
void turn_wiper_on();
void turn_wiper_off();
void turn_porpoise_on();
void turn_porpoise_off();
void start_web_server(bool *thread_running, pthread_t *ws_thread, Web_Server::ws_t *ws_param);
void sig_handler(int signum);

#endif // _ANERIS_HPP

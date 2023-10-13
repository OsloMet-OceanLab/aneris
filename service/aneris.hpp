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

#include "gpio/GPIO.hpp"
#include "Logger/Logger.hpp"
#include "Web_Server/Web_Server.hpp"

/*************/
/* Constants */
/*************/
#define WEB_SERVER_PORT 80

#define SOCKET_PATH "/var/run/aneris.sock"
#define COMMAND_SIZE 2

#define GPIO_LIGHTS 4
#define GPIO_TEST 17
#define GPIO_HYDROPHONE 27
#define GPIO_WIPER 22

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
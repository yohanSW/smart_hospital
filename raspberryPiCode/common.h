#ifndef SH_COMMON
#define SH_COMMON

#include <iostream>
#include <pthread.h>
#include <stdio.h> //for printf
#include <stdint.h> //uint8_t definitions
#include <stdlib.h> //for exit(int);
#include <string.h> //for errno
#include <errno.h> //error output
#include <unistd.h>
#include <fcntl.h>
//wiring Pi
#include <wiringPi.h>
#include <wiringSerial.h>

#define BUTTON	12
#define FAN	21
#define LED_RED 23
#define LED_YELLOW 25
#endif /* SH_COMMON */
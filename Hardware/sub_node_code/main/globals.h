#ifndef GLOBALS_H
#define GLOBALS_H

#include "freertos/FreeRTOS.h"
#include <stdbool.h>

extern TimerHandle_t connection_attempter;
extern volatile bool is_connected;

#endif
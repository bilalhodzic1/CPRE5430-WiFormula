#include "globals.h"

TimerHandle_t connection_attempter = NULL;
volatile bool is_connected = false;
led_strip_handle_t led_strip;
#include "globals.h"
/**
 * @brief global variables to be used by many other files
 */
TimerHandle_t connection_attempter = NULL;
volatile bool is_connected = false;
led_strip_handle_t led_strip;
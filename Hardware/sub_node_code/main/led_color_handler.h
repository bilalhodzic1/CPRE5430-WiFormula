#define NUM_LEDS_ON 60

#include "globals.h"
#include "led_strip.h"
#include "initialize_led_config.h"
void change_led_color_for_flag(int flag_code);
void set_leds_to_value(int r, int g, int b, int num_leds);
#define LED_GPIO 19
#define NUM_LEDS 60 // total LEDs in your strip
#define LED_STRIP_RMT_RES_HZ (10 * 1000 * 1000)
#include "driver/rmt_tx.h"
#include "led_strip.h"
#include "globals.h"

void initialize_led_strip();
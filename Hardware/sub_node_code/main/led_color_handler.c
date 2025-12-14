
#include "led_color_handler.h"
/**
 * @brief Change the LED strip color based on flag value
 *
 * @param flag_code numeric value for the color flag received (1 is green, 2 is red, 3 is yellow)
 */
void change_led_color_for_flag(int flag_code)
{
    // Set variables
    int r, g, b;
    // Switch on flag code and set color
    switch (flag_code)
    {
    case 1:
        r = 0;
        g = 128;
        b = 0;
        set_leds_to_value(r, g, b, NUM_LEDS_ON);
        break;
    case 2:
        r = 128;
        g = 0;
        b = 0;
        set_leds_to_value(r, g, b, NUM_LEDS_ON);

        break;
    case 3:
        r = 120;
        g = 87;
        b = 0;
        set_leds_to_value(r, g, b, NUM_LEDS_ON);

        break;
    default:
        ESP_ERROR_CHECK(led_strip_clear(led_strip));
        break;
    }
}

/**
 * @brief Set a specific number of LEDs to a specific color
 *
 * @param r red value
 * @param g green value
 * @param b blue value
 * @param num_leds number of LEDS to set to this color
 */
void set_leds_to_value(int r, int g, int b, int num_leds)
{
    // Through through indicies from 0 to num_leds and set to color
    int i;
    for (i = 0; i < num_leds; i++)
    {
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, r, g, b));
    }
    // Refresh to push the changes and change the color
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
}
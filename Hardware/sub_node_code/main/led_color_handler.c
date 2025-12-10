
#include "led_color_handler.h"

void change_led_color_for_flag(int flag_code)
{
    int r, g, b;
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

void set_leds_to_value(int r, int g, int b, int num_leds)
{
    int i;
    for (i = 0; i < num_leds; i++)
    {
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, r, g, b));
    }
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
}
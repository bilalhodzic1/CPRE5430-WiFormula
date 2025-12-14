#include "initialize_led_config.h"
/**
 * @brief Initialize LED strip device
 */
void initialize_led_strip()
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,                                  // The GPIO that connected to the LED strip's data line
        .max_leds = NUM_LEDS,                                        // The number of LEDs in the strip,
        .led_model = LED_MODEL_WS2812,                               // LED strip model
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // The color order of the strip: GRB
        .flags = {
            .invert_out = false, // don't invert the output signal
        }};
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
        .mem_block_symbols = 0,                // the memory block size used by the RMT channel
        .flags = {
            .with_dma = 0,
        }};
    // Inittialze new device to global. Clear the strip of all currenly enabled lights
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    ESP_ERROR_CHECK(led_strip_clear(led_strip));
}
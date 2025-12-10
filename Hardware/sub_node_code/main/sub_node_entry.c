#include "main.h"

void app_main(void)
{
    esp_netif_init();
    esp_event_loop_create_default();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &ip_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t station_config = {
        .sta = {
            .ssid = MAIN_NODE_SSID,
            .password = MAIN_NODE_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &station_config));
    ESP_ERROR_CHECK(esp_wifi_start());

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
            .with_dma = 0, // Using DMA can improve performance when driving more LEDs
        }};
    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    printf("MADE LED ON BACKEND\n");
    ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, TEST_LED_INDEX, 5, 5, 5));
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

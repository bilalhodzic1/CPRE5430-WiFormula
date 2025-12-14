#include "main.h"
/**
 * @brief sub node entry point
 */
void app_main(void)
{
    // Intitialze netowrk stack and event loop
    esp_netif_init();
    esp_event_loop_create_default();

    // Initialze nvs memoru for WiFi compatibility
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Setup default wifi in station mode config
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);

    // Register WIFI and IP event hanlders
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

    // Connect to main node wifi from header detail
    wifi_config_t station_config = {
        .sta = {
            .ssid = MAIN_NODE_SSID,
            .password = MAIN_NODE_PASS,
        },
    };
    // Start WIFI and initialize hardware component
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &station_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    initialize_led_strip();
}

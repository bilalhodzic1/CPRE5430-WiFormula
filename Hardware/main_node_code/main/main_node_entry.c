#include "main.h"

/**
 * @brief ESP32 entry point
 *
 */
void app_main(void)
{
    // Initialize network stack and event loop
    esp_netif_init();
    esp_event_loop_create_default();

    // Intiailze persistent flash memory for WiFi compatibility
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    // Start WIFI apsta
    start_apsta_mode();
    // Initialize HTTP request task
    xTaskCreate(
        http_task,
        "http_task",
        24576,
        NULL,
        5,
        &http_task_handle);
}
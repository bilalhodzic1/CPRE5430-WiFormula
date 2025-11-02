#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "lwip/inet.h"

static void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case WIFI_EVENT_AP_STACONNECTED:
        wifi_event_ap_staconnected_t *connect_event = (wifi_event_ap_staconnected_t *)data;
        printf("Station " MACSTR " connected\n", MAC2STR(connect_event->mac));
        break;
    case WIFI_EVENT_AP_STADISCONNECTED:
        wifi_event_ap_stadisconnected_t *disconnect_event = (wifi_event_ap_stadisconnected_t *)data;
        printf("Station " MACSTR " disconnected\n", MAC2STR(disconnect_event->mac));
        break;
    default:
        printf("event caught %s %ld\n", base, id);
        break;
    }
}

static void ip_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case IP_EVENT_AP_STAIPASSIGNED:
        ip_event_ap_staipassigned_t *ap_ip_assignment_event = (ip_event_ap_staipassigned_t *)data;
        printf("Station " MACSTR " assigned to ip address " IPSTR, MAC2STR(ap_ip_assignment_event->mac),
               IP2STR(&ap_ip_assignment_event->ip));
        break;
    default:
        break;
    }
}

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

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler, NULL, NULL));

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_config));

    wifi_config_t ap_config = {
        .ap = {
            .ssid = "wi-formula-test",
            .ssid_len = 0,
            .channel = 1,
            .password = "test_pass",
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK},
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
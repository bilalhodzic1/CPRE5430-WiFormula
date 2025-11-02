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

static void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data){
    switch(id){
        case WIFI_EVENT_AP_STACONNECTED:
            wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t*) data;
            printf("Station " MACSTR " connected\n", MAC2STR(event->mac));
            break;
        case WIFI_EVENT_AP_STADISCONNECTED:
            wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t*) data;
            printf("Station " MACSTR " disconnected\n", MAC2STR(event->mac));
            break;
        default:
            printf("event caught %s\n", base);
    }
}

void app_main(void){
    esp_netif_init();
    esp_event_loop_create_default();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}
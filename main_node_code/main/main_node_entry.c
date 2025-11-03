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
#include "wifi_config_local.h"
#include "mosq_broker.h"
#include "mqtt_client.h"
static TimerHandle_t connection_attempter = NULL;
static volatile bool is_connected = false;

typedef struct mosq_broker_config mosq_broker_config_t;

static volatile bool is_broker_started = false;

static void start_mqtt_broker(void *args)
{
    is_broker_started = true;
    mosq_broker_config_t mqtt_config = {
        .host = "0.0.0.0",
        .port = 1883,
        .tls_cfg = NULL};
    mosq_broker_run(&mqtt_config);
    vTaskDelete(NULL);
}

static void mqtt_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case MQTT_EVENT_CONNECTED:
        printf("Connected to MQTT successfully\n");
        break;
    default:
        break;
    }
}

static void start_local_client()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.hostname = "127.0.0.1",
        .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,
        .broker.address.port = 1883,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

static void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case WIFI_EVENT_AP_STACONNECTED:
        wifi_event_ap_staconnected_t *connect_event = (wifi_event_ap_staconnected_t *)data;
        printf("AP MODE: Station " MACSTR " connected\n", MAC2STR(connect_event->mac));
        break;
    case WIFI_EVENT_AP_STADISCONNECTED:
        wifi_event_ap_stadisconnected_t *disconnect_event = (wifi_event_ap_stadisconnected_t *)data;
        printf("AP MODE: Station " MACSTR " disconnected\n", MAC2STR(disconnect_event->mac));
        break;
    case WIFI_EVENT_STA_START:
        printf("STATION MODE: Starting station connection\n");
        esp_wifi_connect();
        break;
    case WIFI_EVENT_AP_START:
        printf("AP MODE: Successfully started. Initiallizing broker\n");
        if (!is_broker_started)
        {
            xTaskCreate(start_mqtt_broker, "start_broker", 16384, NULL, 5, NULL);
        }
        start_local_client();
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
        printf("AP MODE: Station " MACSTR " assigned to ip address " IPSTR "\n", MAC2STR(ap_ip_assignment_event->mac),
               IP2STR(&ap_ip_assignment_event->ip));
        break;
    case IP_EVENT_STA_GOT_IP:
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)data;
        printf("STATION MODE: Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
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

    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    esp_netif_create_default_wifi_ap();
    wifi_config_t ap_config = {
        .ap = {
            .ssid = "wi-formula-test",
            .ssid_len = 0,
            .channel = 1,
            .password = "test_pass",
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK},
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

    esp_netif_create_default_wifi_sta();
    wifi_config_t sta_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS}};

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));

    ESP_ERROR_CHECK(esp_wifi_start());
}
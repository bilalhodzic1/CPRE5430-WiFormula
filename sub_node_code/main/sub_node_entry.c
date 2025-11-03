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
#include "mqtt_client.h"

static TimerHandle_t connection_attempter = NULL;
static volatile bool is_connected = false;

static void attempt_connection_cb()
{
    printf("DEBUG: Attempting connection\n");
    if (!is_connected)
    {
        esp_wifi_connect();
    }
}

static void start_connection_attempt_timer()
{
    if (!connection_attempter)
    {
        connection_attempter = xTimerCreate("attempter",
                                            pdMS_TO_TICKS(10 * 1000),
                                            pdTRUE, NULL, attempt_connection_cb);
    }
    xTimerStart(connection_attempter, 0);
}

static void stop_connection_attempt_timer()
{
    if (connection_attempter)
        xTimerStop(connection_attempter, 0);
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

static void start_local_client(esp_ip4_addr_t *gw)
{
    static char host[16];
    snprintf(host, sizeof(host), IPSTR, IP2STR(gw));

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.hostname = host,
        .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,
        .broker.address.port = 1883,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

static void ip_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case IP_EVENT_STA_GOT_IP:
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)data;
        printf("Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
        printf("AP IP address is " IPSTR " \n", IP2STR(&event->ip_info.gw));
        is_connected = true;
        stop_connection_attempt_timer();
        start_local_client(&event->ip_info.gw);
        break;
    default:
        break;
    }
}

static void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case WIFI_EVENT_STA_START:
        start_connection_attempt_timer();
        break;
    case WIFI_EVENT_STA_CONNECTED:
        wifi_event_sta_connected_t *sta_connected_event = (wifi_event_sta_connected_t *)data;
        printf("Station connected to AP with bssid " MACSTR "  \n", MAC2STR(sta_connected_event->bssid));
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        is_connected = false;
        start_connection_attempt_timer();
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
            .ssid = "wi-formula-test",
            .password = "test_pass",
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &station_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

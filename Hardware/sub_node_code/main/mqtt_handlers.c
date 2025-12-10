#include "mqtt_handlers.h"

void start_local_client(esp_ip4_addr_t *gw)
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

void mqtt_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    esp_mqtt_event_handle_t event = data;
    esp_mqtt_client_handle_t client = event->client;
    switch (id)
    {
    case MQTT_EVENT_CONNECTED:
        printf("Connected to MQTT successfully\n");
        esp_mqtt_client_subscribe(client, TOPIC_ENDPOINT, 0);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        printf("Successfully subscribed\n");
        break;
    case MQTT_EVENT_DATA:
        int number = atoi((char *)event->data);
        switch (number)
        {
        case 1:
            ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, TEST_LED_INDEX, 128, 0, 0));
            ESP_ERROR_CHECK(led_strip_refresh(led_strip));
            break;
        case 2:
            ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, TEST_LED_INDEX, 0, 128, 0));
            ESP_ERROR_CHECK(led_strip_refresh(led_strip));
            break;
        case 3:
            ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, TEST_LED_INDEX, 87, 120, 0));
            ESP_ERROR_CHECK(led_strip_refresh(led_strip));
            break;
        default:
            ESP_ERROR_CHECK(led_strip_clear(led_strip));
            break;
        }
        break;
    default:
        break;
    }
}

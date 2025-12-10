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
        esp_mqtt_client_subscribe(client, "home/random", 0);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        printf("Successfully subscribed\n");
        break;
    case MQTT_EVENT_DATA:
        int number = atoi((char *)event->data);
        uint8_t r = (number * 5) % 128;
        uint8_t g = (number * 3) % 128;
        uint8_t b = (number * 7) % 128;

        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, TEST_LED_INDEX, g, r, b));
        ESP_ERROR_CHECK(led_strip_refresh(led_strip));
        break;
    default:
        break;
    }
}

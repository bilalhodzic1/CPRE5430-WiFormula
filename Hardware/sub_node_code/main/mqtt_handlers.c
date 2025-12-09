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
        printf("TOPIC=%.*s\n", event->topic_len, event->topic);
        printf("DATA=%.*s\n", event->data_len, event->data);
        break;
    default:
        break;
    }
}

#include "mqtt_handlers.h"

/**
 * @brief Start local MQTT client to broker
 *
 * @param gw main node IP address for connecting to MQTT broker hosted there
 */
void start_local_client(esp_ip4_addr_t *gw)
{
    // Set host varaible to correct IP address string
    static char host[16];
    snprintf(host, sizeof(host), IPSTR, IP2STR(gw));

    // Initialize config to correct host and spec
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.hostname = host,
        .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,
        .broker.address.port = 1883,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    // Register event handler and start client
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

/**
 * @brief Handles MQTT client events
 *
 * @param arg optional arguement param
 * @param base Type of event received (IP WIFI Etc.)
 * @param id Event ID distingusihing what MQTT event it is
 * @param data data payload of event
 */
void mqtt_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    // Set data and client handles
    esp_mqtt_event_handle_t event = data;
    esp_mqtt_client_handle_t client = event->client;

    // Conduct different action depending on event
    switch (id)
    {
    case MQTT_EVENT_CONNECTED:
        // On connecton to broker subscribe to correct topic defined in header
        printf("Connected to MQTT successfully\n");
        esp_mqtt_client_subscribe(client, TOPIC_ENDPOINT, 0);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        // Log successful subscription
        printf("Successfully subscribed\n");
        break;
    case MQTT_EVENT_DATA:
        // On data received change flag color to match
        int number = atoi((char *)event->data);
        change_led_color_for_flag(number);
        break;
    default:
        break;
    }
}

#include "mqtt_event_handler.h"
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
    // Switch on MQTT event ID
    switch (id)
    {
    case MQTT_EVENT_CONNECTED:
        // If connected to broker successfully connect to websocket for data
        printf("Connected to MQTT successfully\n");
        start_websocket();
        break;
    case MQTT_EVENT_PUBLISHED:
        // On successful publish log detail
        esp_mqtt_event_handle_t event = data;
        printf("Successfully published. Message id = %d\n", event->msg_id);
        break;
    default:
        break;
    }
}

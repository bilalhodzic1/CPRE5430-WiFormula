#include "mqtt_event_handler.h"

void mqtt_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case MQTT_EVENT_CONNECTED:
        printf("Connected to MQTT successfully\n");
        start_websocket();
        break;
    case MQTT_EVENT_PUBLISHED:
        esp_mqtt_event_handle_t event = data;
        printf("Successfully published. Message id = %d\n", event->msg_id);
        break;
    default:
        break;
    }
}

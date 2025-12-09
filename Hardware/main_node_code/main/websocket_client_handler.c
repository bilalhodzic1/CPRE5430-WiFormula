#include "websocket_client_handler.h"

void websocket_client_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case WEBSOCKET_EVENT_CONNECTED:
        printf("Websocket connected successfully\n");
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        printf("Websocket disconnected!\n");
        break;
    case WEBSOCKET_EVENT_DATA:
    {
        esp_websocket_event_data_t *event = (esp_websocket_event_data_t *)data;
        if (event->op_code != 1)
        {
            printf("Ignoring opcode %d\n", event->op_code);
            break;
        }
        if (event->data_len <= 0)
        {
            break;
        }
        const char *payload = event->data_ptr + event->payload_offset;
        int len = event->payload_len;
        printf("len %d\n", len);
        char cleaned_payload[len + 1];
        memcpy(cleaned_payload, payload, len);
        cleaned_payload[len] = '\0';
        printf("Payload %s\n", cleaned_payload);
        esp_mqtt_client_publish(
            client,
            "home/random",
            cleaned_payload,
            len + 1,
            0,
            0);
        break;
    }
    default:
        printf("Some other event occured\n");
        break;
    }
}
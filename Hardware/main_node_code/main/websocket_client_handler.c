#include "websocket_client_handler.h"
/**
 * @brief Event handler for websocket events
 *
 * @param arg optional arguement param
 * @param base Type of event received (IP WIFI Etc.)
 * @param id Event ID distingusihing what WS event it is
 * @param data data payload of event
 */
void websocket_client_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    // Different action depending on event
    switch (id)
    {
    case WEBSOCKET_EVENT_CONNECTED:
        // Log succesful connection
        printf("Websocket connected successfully\n");
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        // Log disconection
        printf("Websocket disconnected!\n");
        break;
    case WEBSOCKET_EVENT_DATA:
    {
        // On data received check if real payload
        esp_websocket_event_data_t *event = (esp_websocket_event_data_t *)data;
        // Opcodes one and 10 are for control and have no data
        if (event->op_code != 1)
        {
            printf("Ignoring opcode %d\n", event->op_code);
            break;
        }
        if (event->data_len <= 0)
        {
            break;
        }
        // Convert payload to local JSON object
        const char *payload = event->data_ptr + event->payload_offset;
        int len = event->payload_len;
        printf("len %d\n", len);
        char cleaned_payload[len + 1];
        memcpy(cleaned_payload, payload, len);
        cleaned_payload[len] = '\0';
        printf("Payload %s\n", cleaned_payload);

        // Parse JSON check for success
        cJSON *root = cJSON_Parse(cleaned_payload);
        if (!root)
        {
            printf("JSON parse error!\n");
            break;
        }
        // Check if flag value exists
        cJSON *flag_value = cJSON_GetObjectItem(root, "flag_value");
        if (flag_value != NULL)
        {
            // If flag color value exists push to MQTT flags topic for sub nodes
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", flag_value->valueint);
            esp_mqtt_client_publish(
                client,
                "home/flags",
                buf,
                len + 1,
                0,
                0);
        }
        // Free JSON space
        cJSON_Delete(root);
        break;
    }
    default:
        printf("Some other event occured\n");
        break;
    }
}
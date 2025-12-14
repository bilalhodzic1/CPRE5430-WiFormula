#include "wifi_event_handler.h"

/**
 * @brief Handles WIFI events
 *
 * @param arg optional arguement param
 * @param base Type of event received (IP WIFI Etc.)
 * @param id Event ID distingusihing what WIFI event it is
 * @param data data payload of event
 */
void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    // Switch on different events to do different things
    switch (id)
    {
    case WIFI_EVENT_STA_START:
        // When station mode is ready attempt to connect to wifi
        start_connection_attempt_timer();
        break;
    case WIFI_EVENT_STA_CONNECTED:
        // On wifi connetion log
        wifi_event_sta_connected_t *sta_connected_event = (wifi_event_sta_connected_t *)data;
        printf("Station connected to AP with bssid " MACSTR "  \n", MAC2STR(sta_connected_event->bssid));
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        // On disconnection retry connections
        is_connected = false;
        start_connection_attempt_timer();
        break;
    default:
        break;
    }
}
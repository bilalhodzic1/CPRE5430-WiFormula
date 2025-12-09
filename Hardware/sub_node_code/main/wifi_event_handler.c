#include "wifi_event_handler.h"

void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
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
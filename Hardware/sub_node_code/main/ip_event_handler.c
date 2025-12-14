#include "ip_event_handler.h"
/**
 * @brief Handles IP network events
 *
 * @param arg optional arguement param
 * @param base Type of event received (IP WIFI Etc.)
 * @param id Event ID distingusihing what IP event it is
 * @param data data payload of event
 */
void ip_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    // Take different action depending on event
    switch (id)
    {
    case IP_EVENT_STA_GOT_IP:
        // Once station gets IP address set connected flag stop timer and connect start MQTT client
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)data;
        printf("Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
        printf("AP IP address is " IPSTR " \n", IP2STR(&event->ip_info.gw));
        is_connected = true;
        stop_connection_attempt_timer();
        start_local_client(&event->ip_info.gw);
        break;
    default:
        break;
    }
}
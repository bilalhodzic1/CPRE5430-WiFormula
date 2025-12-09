#include "ip_event_handler.h"

void ip_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case IP_EVENT_STA_GOT_IP:
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
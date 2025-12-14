#include "ip_event_handler.h"
/**
 * @brief Event handler for handling IP events
 *
 * @param arg optional arguement param
 * @param base Type of event received (IP WIFI Etc.)
 * @param id Event ID distingusihing what IP event it is
 * @param data data payload of event
 */
void ip_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    // Different IDs correspond to different actions
    switch (id)
    {
    case IP_EVENT_AP_STAIPASSIGNED:
        // Case station connected to local AP IP assignment to coorect payload. Log details
        ip_event_ap_staipassigned_t *ap_ip_assignment_event = (ip_event_ap_staipassigned_t *)data;
        printf("AP MODE: Station " MACSTR " assigned to ip address " IPSTR "\n", MAC2STR(ap_ip_assignment_event->mac),
               IP2STR(&ap_ip_assignment_event->ip));
        break;
    case IP_EVENT_STA_GOT_IP:
        // Station mode of device got IP. Begin controller auth request pipeline
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)data;
        printf("STATION MODE: Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
        http_request_number = 1;
        xTaskNotifyGive(http_task_handle);
        break;
    default:
        break;
    }
}
#include "ip_event_handler.h"

void ip_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case IP_EVENT_AP_STAIPASSIGNED:
        ip_event_ap_staipassigned_t *ap_ip_assignment_event = (ip_event_ap_staipassigned_t *)data;
        printf("AP MODE: Station " MACSTR " assigned to ip address " IPSTR "\n", MAC2STR(ap_ip_assignment_event->mac),
               IP2STR(&ap_ip_assignment_event->ip));
        break;
    case IP_EVENT_STA_GOT_IP:
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)data;
        printf("STATION MODE: Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
        http_request_number = 1;
        xTaskNotifyGive(http_task_handle);
        break;
    default:
        break;
    }
}
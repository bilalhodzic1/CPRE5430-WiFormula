#include "wifi_event_handler.h"

void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    switch (id)
    {
    case WIFI_EVENT_AP_STACONNECTED:
        printf("Heres to hoping\n");
        wifi_event_ap_staconnected_t *connect_event = (wifi_event_ap_staconnected_t *)data;
        esp_netif_dhcps_stop(ap_netif);
        if (!mac_pending)
        {
            printf("AP MODE: Station " MACSTR " connected\n", MAC2STR(connect_event->mac));
            memcpy(pending_mac_bytes, connect_event->mac, 6);
            pending_aid = connect_event->aid;
            mac_pending = true;
            http_request_number = 2;
            xTaskNotifyGive(http_task_handle);
        }
        else
        {
            esp_wifi_deauth_sta(connect_event->aid);
        }
        break;
    case WIFI_EVENT_AP_STADISCONNECTED:
        wifi_event_ap_stadisconnected_t *disconnect_event = (wifi_event_ap_stadisconnected_t *)data;
        printf("AP MODE: Station " MACSTR " disconnected\n", MAC2STR(disconnect_event->mac));
        break;
    case WIFI_EVENT_STA_START:
        printf("STATION MODE: Starting station connection\n");
        esp_wifi_connect();
        break;
    case WIFI_EVENT_AP_START:
        printf("AP MODE: Successfully started. Initiallizing broker\n");
        if (!is_broker_started)
        {
            xTaskCreate(start_mqtt_broker, "start_broker", 16384, NULL, 5, NULL);
        }
        start_local_client();
        break;
    default:
        printf("event caught %s %ld\n", base, id);
        break;
    }
}
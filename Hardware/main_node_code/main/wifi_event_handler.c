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
    // Switch depending on event type
    switch (id)
    {
    case WIFI_EVENT_AP_STACONNECTED:
        // When station connects to AP initiate AUTH process
        wifi_event_ap_staconnected_t *connect_event = (wifi_event_ap_staconnected_t *)data;
        // Stop DHCP server
        esp_netif_dhcps_stop(ap_netif);
        if (!mac_pending)
        {
            // Send mac to http request to handle auth
            printf("AP MODE: Station " MACSTR " connected\n", MAC2STR(connect_event->mac));
            memcpy(pending_mac_bytes, connect_event->mac, 6);
            pending_aid = connect_event->aid;
            mac_pending = true;
            http_request_number = 2;
            xTaskNotifyGive(http_task_handle);
        }
        else
        {
            // If one is pending immediate de auth. No support for concurrent authorization
            esp_wifi_deauth_sta(connect_event->aid);
        }
        break;
    case WIFI_EVENT_AP_STADISCONNECTED:
        // When a station disconnects log
        wifi_event_ap_stadisconnected_t *disconnect_event = (wifi_event_ap_stadisconnected_t *)data;
        printf("AP MODE: Station " MACSTR " disconnected\n", MAC2STR(disconnect_event->mac));
        break;
    case WIFI_EVENT_STA_START:
        // On local station node connect to wifi
        printf("STATION MODE: Starting station connection\n");
        esp_wifi_connect();
        break;
    case WIFI_EVENT_AP_START:
        // On local AP started. Start MQTT broker and local client
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
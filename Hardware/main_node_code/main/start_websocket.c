#include "start_websocket.h"

void start_websocket()
{
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);

    char ws_url[128];
    snprintf(ws_url, sizeof(ws_url),
             "ws://207.211.177.254:8080/formula-data-stream?device=ESP32&mac=%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    esp_websocket_client_config_t ws_cfg = {
        .uri = ws_url};

    ws_client = esp_websocket_client_init(&ws_cfg);
    esp_websocket_register_events(ws_client,
                                  WEBSOCKET_EVENT_ANY, websocket_client_event_handler, NULL);

    esp_websocket_client_start(ws_client);
}
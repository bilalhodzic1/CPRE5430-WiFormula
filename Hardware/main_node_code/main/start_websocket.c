#include "start_websocket.h"

void start_websocket()
{
    esp_websocket_client_config_t ws_cfg = {
        .uri = WS_URI,
    };

    ws_client = esp_websocket_client_init(&ws_cfg);
    esp_websocket_register_events(ws_client,
                                  WEBSOCKET_EVENT_ANY, websocket_client_event_handler, NULL);

    esp_websocket_client_start(ws_client);
}
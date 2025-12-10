#include "esp_websocket_client.h"
#include "globals.h"
#include "cJSON.h"

void websocket_client_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data);

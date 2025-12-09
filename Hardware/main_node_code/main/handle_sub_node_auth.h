#include "globals.h"
#include "esp_http_client.h"
#include "http_event_handler.h"
#include "esp_wifi.h"

void create_device_request(const char *mac_address);
void handle_sub_node_auth(const char *mac_address);

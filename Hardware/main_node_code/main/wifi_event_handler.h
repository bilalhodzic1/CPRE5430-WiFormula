#include "globals.h"
#include "esp_wifi.h"
#include "mqtt_startups.h"
#include "esp_mac.h"

void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data);

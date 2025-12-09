#include "esp_wifi.h"
#include "globals.h"
#include "connection_attempter.h"
#include "esp_mac.h"

void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data);

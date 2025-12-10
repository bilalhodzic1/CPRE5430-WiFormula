#include "mqtt_client.h"
#include "esp_wifi.h"
#include "led_strip.h"
#include "globals.h"
#include "initialize_led_config.h"

void mqtt_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data);
void start_local_client(esp_ip4_addr_t *gw);

#define TOPIC_ENDPOINT "home/flags"

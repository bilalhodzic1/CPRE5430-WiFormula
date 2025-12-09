#include "globals.h"
#include "mosq_broker.h"
#include "esp_wifi.h"
#include "mqtt_event_handler.h"

void start_local_client();
void start_mqtt_broker(void *args);

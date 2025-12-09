#ifndef GLOBALS_H
#define GLOBALS_H
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "mqtt_client.h"
#include "esp_netif.h"
#include "esp_websocket_client.h"

extern int http_request_number;
typedef struct mosq_broker_config mosq_broker_config_t;
extern volatile bool is_broker_started;
extern TaskHandle_t http_task_handle;
extern esp_mqtt_client_handle_t client;
extern esp_netif_t *ap_netif;
extern uint8_t pending_mac_bytes[6];
extern uint16_t pending_aid;
extern bool mac_pending;
extern esp_websocket_client_handle_t ws_client;
#endif
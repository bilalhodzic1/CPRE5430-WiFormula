#include "globals.h"

int http_request_number = 0;
typedef struct mosq_broker_config mosq_broker_config_t;
volatile bool is_broker_started = false;
TaskHandle_t http_task_handle = NULL;
esp_mqtt_client_handle_t client = NULL;
esp_netif_t *ap_netif = NULL;
uint8_t pending_mac_bytes[6];
uint16_t pending_aid = 0;
bool mac_pending = false;
esp_websocket_client_handle_t ws_client;

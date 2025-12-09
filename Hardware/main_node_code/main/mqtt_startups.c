#include "mqtt_startups.h"

void start_mqtt_broker(void *args)
{
    is_broker_started = true;
    mosq_broker_config_t mqtt_config = {
        .host = "0.0.0.0",
        .port = 1883,
        .tls_cfg = NULL};
    mosq_broker_run(&mqtt_config);
    vTaskDelete(NULL);
}

void start_local_client()
{
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);

    static char mac_str[18];
    sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.hostname = "127.0.0.1",
        .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,
        .broker.address.port = 1883,
        .credentials.username = mac_str};
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}
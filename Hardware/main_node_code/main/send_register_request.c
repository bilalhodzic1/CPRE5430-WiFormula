
#include "send_register_request.h"

void send_register_request()
{
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_AP, mac);

    char mac_str[18];
    sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    char post_data[128];
    sprintf(post_data,
            "{\"mac_address\":\"%s\",\"device_type\":\"C\"}",
            mac_str);

    char local_response_buffer[4096 + 1] = {0};
    esp_http_client_config_t config = {
        .host = "207.211.177.254",
        .port = 8080,
        .path = "/api/device/register",
        .method = HTTP_METHOD_POST,
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer, // Pass address of local buffer to get response
        .disable_auto_redirect = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        int response_code = esp_http_client_get_status_code(client);
        if (response_code == 400)
        {
            printf("Already Registered\n");
        }
        else if (response_code == 201)
        {
            printf("Successfully registered\n");
        }
    }
    else
    {
        printf("Test failed\n");
    }
    esp_http_client_cleanup(client);
}
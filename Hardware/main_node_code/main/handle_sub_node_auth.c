#include "handle_sub_node_auth.h"

void handle_sub_node_auth(const char *mac_address)
{
    char path[128];
    sprintf(path, "/api/device/auth/%s", mac_address);
    char local_response_buffer[4096 + 1] = {0};
    esp_http_client_config_t config = {
        .host = "207.211.177.254",
        .port = 8080,
        .path = path,
        .method = HTTP_METHOD_GET,
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer, // Pass address of local buffer to get response
        .disable_auto_redirect = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_perform(client);

    int response_code = esp_http_client_get_status_code(client);
    if (response_code == 404)
    {
        create_device_request(mac_address);
        esp_wifi_deauth_sta(pending_aid);
    }
    else if (response_code == 200)
    {
        printf("Allowed MAC address to connect : %s\n", mac_address);
    }
    else if (response_code == 401)
    {
        printf("Mac address %s not yet authenticated\n", mac_address);
        esp_wifi_deauth_sta(pending_aid);
    }
    else
    {
        printf("Unexpected outcome. ERROR\n");
    }

    mac_pending = false;
    esp_netif_dhcps_start(ap_netif);
    esp_http_client_cleanup(client);
}
void create_device_request(const char *mac_address)
{
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_AP, mac);

    char mac_str[18];
    sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    char post_data[256];
    sprintf(post_data,
            "{\"mac_address\":\"%s\","
            "\"controller_mac_address\":\"%s\","
            "\"device_type\":\"S\"}",
            mac_address, mac_str);
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
            printf("Successfully requested\n");
        }
    }
    else
    {
        printf("HTTP failed\n");
    }
    esp_http_client_cleanup(client);
}

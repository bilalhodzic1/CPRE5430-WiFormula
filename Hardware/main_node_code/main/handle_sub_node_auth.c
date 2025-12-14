#include "handle_sub_node_auth.h"
/**
 * @brief Handles authentication of a sub node mac address
 *
 * @param mac_address A character string of a sub node mac address
 */
void handle_sub_node_auth(const char *mac_address)
{
    // Set path to check if sub node is authorized for system
    char path[128];
    sprintf(path, "/api/device/auth/%s", mac_address);
    // Set up and make API call to check auth
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
        // If response if 404 no such registration request exists therefore create a new on and de-authenticate the device
        create_device_request(mac_address);
        esp_wifi_deauth_sta(pending_aid);
    }
    else if (response_code == 200)
    {
        // If response is 200 device is okay to proceed. Log and do nothing
        printf("Allowed MAC address to connect : %s\n", mac_address);
    }
    else if (response_code == 401)
    {
        // If response is 401 device request has not been approved yet. Deauthenticate device
        printf("Mac address %s not yet authenticated\n", mac_address);
        esp_wifi_deauth_sta(pending_aid);
    }
    else
    {
        // Other response is an error
        printf("Unexpected outcome. ERROR\n");
    }

    // Mac address has been processed
    mac_pending = false;
    // Start DHCP again to assign IP to authenticated device. Free http client
    esp_netif_dhcps_start(ap_netif);
    esp_http_client_cleanup(client);
}
/**
 * @brief Sends a registration request to the server for a new device
 *
 * @param mac_address First integer
 */
void create_device_request(const char *mac_address)
{
    // Setup POST request body for new sub node
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
    // Make HTTP request
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
            // If 400 we already registered this device
            printf("Already Registered\n");
        }
        else if (response_code == 201)
        {
            // If 201 the request is created
            printf("Successfully requested\n");
        }
    }
    else
    {
        // Other responses are errors
        printf("HTTP failed\n");
    }
    // Cleanup the client
    esp_http_client_cleanup(client);
}

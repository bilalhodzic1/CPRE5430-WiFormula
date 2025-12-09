#include "http_event_handler.h"

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static int output_len;
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        if (output_len == 0 && evt->user_data)
        {
            memset(evt->user_data, 0, 4096);
        }
        break;
    default:
        break;
    }
    return ESP_OK;
}
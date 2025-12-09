idf_component_register(SRCS "start_websocket.c" "websocket_client_handler.c" "globals.c" "main_node_entry.c"
                       PRIV_REQUIRES esp_wifi nvs_flash mqtt esp_http_client json
                       INCLUDE_DIRS "")
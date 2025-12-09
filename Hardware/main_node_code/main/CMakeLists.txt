idf_component_register(SRCS "handle_sub_node_auth.c" "send_register_request.c" "http_task.c" "http_event_handler.c" "start_websocket.c" "websocket_client_handler.c" "globals.c" "main_node_entry.c"
                       PRIV_REQUIRES esp_wifi nvs_flash mqtt esp_http_client json
                       INCLUDE_DIRS "")
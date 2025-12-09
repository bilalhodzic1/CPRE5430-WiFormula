idf_component_register(SRCS "mqtt_handlers.c" "wifi_event_handler.c" "ip_event_handler.c" "connection_attempter.c" "globals.c" "sub_node_entry.c"
                       PRIV_REQUIRES esp_wifi mqtt nvs_flash
                       INCLUDE_DIRS "")
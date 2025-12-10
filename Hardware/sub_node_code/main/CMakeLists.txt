idf_component_register(SRCS "led_color_handler.c" "intitialize_led_config.c" "mqtt_handlers.c" "wifi_event_handler.c" "ip_event_handler.c" "connection_attempter.c" "globals.c" "sub_node_entry.c"
                       PRIV_REQUIRES esp_wifi mqtt nvs_flash esp_driver_rmt led_strip
                       INCLUDE_DIRS "")
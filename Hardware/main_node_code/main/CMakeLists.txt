idf_component_register(SRCS "globals.c" "main_node_entry.c"
                       PRIV_REQUIRES esp_wifi nvs_flash mqtt esp_http_client json
                       INCLUDE_DIRS "")
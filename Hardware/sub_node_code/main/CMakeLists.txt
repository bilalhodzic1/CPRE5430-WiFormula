idf_component_register(SRCS "connection_attempter.c" "globals.c" "sub_node_entry.c"
                       PRIV_REQUIRES esp_wifi mqtt nvs_flash
                       INCLUDE_DIRS "")
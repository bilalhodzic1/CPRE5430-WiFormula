#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "lwip/inet.h"
#include "mqtt_client.h"
#include "globals.h"
#include "connection_attempter.h"
#include "wifi_event_handler.h"
#include "ip_event_handler.h"
#include "wifi_config_local.h"
#include "initialize_led_config.h"
#endif
#include "globals.h"
#include "freertos/task.h"
#include "esp_wifi.h"

void attempt_connection_cb();
void start_connection_attempt_timer();
void stop_connection_attempt_timer();
#include "connection_attempter.h"

void attempt_connection_cb()
{
    printf("DEBUG: Attempting connection\n");
    if (!is_connected)
    {
        esp_wifi_connect();
    }
}

void start_connection_attempt_timer()
{
    if (!connection_attempter)
    {
        connection_attempter = xTimerCreate("attempter",
                                            pdMS_TO_TICKS(10 * 1000),
                                            pdTRUE, NULL, attempt_connection_cb);
    }
    xTimerStart(connection_attempter, 0);
}

void stop_connection_attempt_timer()
{
    if (connection_attempter)
        xTimerStop(connection_attempter, 0);
}

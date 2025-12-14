#include "connection_attempter.h"
/**
 * @brief Connection attempt callback function used in task loop
 */
void attempt_connection_cb()
{
    // If global connected is false try again
    printf("DEBUG: Attempting connection\n");
    if (!is_connected)
    {
        esp_wifi_connect();
    }
}

/**
 * @brief Timer task to start connection attempt every 10 seconds
 */
void start_connection_attempt_timer()
{
    // If the task is not yet created then make it
    if (!connection_attempter)
    {
        connection_attempter = xTimerCreate("attempter",
                                            pdMS_TO_TICKS(10 * 1000),
                                            pdTRUE, NULL, attempt_connection_cb);
    }
    // Start the timer task
    xTimerStart(connection_attempter, 0);
}

/**
 * @brief Stop the connection attempter timer
 */
void stop_connection_attempt_timer()
{
    // If the timer exists stop it
    if (connection_attempter)
        xTimerStop(connection_attempter, 0);
}

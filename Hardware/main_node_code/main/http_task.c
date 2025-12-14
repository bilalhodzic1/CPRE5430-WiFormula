#include "http_task.h"
/**
 * @brief Task loop for making HTTP calls off the main thread. Waits for signal from other code
 *
 * @param pv optional param for passing data to task
 */
void http_task(void *pv)
{
    // Infitie loop that waits for signal
    for (;;)
    {
        // Wait for signal
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Task num set by other methods. On notify do correct HTTP call based on number
        switch (http_request_number)
        {
        case 1:
            // Send main node registration request
            send_register_request();
            break;
        case 2:
            // Setup and handle sub node auth
            if (mac_pending)
            {
                char pending_mac_str[18];

                sprintf(pending_mac_str,
                        "%02X:%02X:%02X:%02X:%02X:%02X",
                        pending_mac_bytes[0],
                        pending_mac_bytes[1],
                        pending_mac_bytes[2],
                        pending_mac_bytes[3],
                        pending_mac_bytes[4],
                        pending_mac_bytes[5]);
                handle_sub_node_auth(pending_mac_str);
                mac_pending = false;
            }
            break;
        default:
            // Invalid number
            printf("Unknown request number: %d\n", http_request_number);
            break;
        }
    }
}
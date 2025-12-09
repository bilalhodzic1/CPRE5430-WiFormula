#include "http_task.h"

void http_task(void *pv)
{
    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        switch (http_request_number)
        {
        case 1:
            send_register_request();
            break;
        case 2:
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
            printf("Unknown request number: %d\n", http_request_number);
            break;
        }
    }
}
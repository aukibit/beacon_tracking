#include "esp_event_loop.h"

#include "apps/sntp/sntp.h"
#include "freertos/event_groups.h"

extern EventGroupHandle_t wifi_event_group;

extern const int CONNECTED_BIT;

void set_time(void) {
    // TODO: Finish this so it works. 
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                        false, true, portMAX_DELAY);

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}
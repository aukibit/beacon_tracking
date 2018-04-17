#include "esp_event_loop.h"

#include "apps/sntp/sntp.h"
#include "freertos/event_groups.h"

extern EventGroupHandle_t wifi_event_group;

const int CONNECTED_BIT = BIT0;

void set_time(void) {
    // TODO: Finish this so it works. 
    // https://github.com/espressif/esp-idf/blob/master/examples/protocols/sntp/main/sntp_example_main.c
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                        false, true, portMAX_DELAY);

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}
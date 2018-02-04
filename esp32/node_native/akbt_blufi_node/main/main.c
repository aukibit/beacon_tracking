#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "ble_iface.h"

void app_main() {

    ble_init();
    while (true) {
        printf("<3 - HEARTBEAT - <3\n");
        
        fflush(stdout);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "wifi_manager.h"
#include "display.h"
#include "lvgl_demo.h"


#define TFT_BL 27  // Backlight pin
#define TFT_WIDTH 240
#define TFT_HEIGHT 320
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15

static const char *TAG = "kidsbox";
char ip_str[16];

static void nvs_init_or_recover(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
}



void app_main(void)
{
    printf("\n\n=== KIDSBOX NEW BUILD %s %s ===\n\n", __DATE__, __TIME__);
    
    nvs_init_or_recover();

    wifi_manager_start_sta();
    printf("Finished WiFi Initialization\n");
    printf("Initiating Display...\n");
    display_init(); // Initialize the display hardware
    printf("Display Initialized\n");
    printf("Running WIFI Status\n");
    wifi_manager_status_t st = wifi_manager_get_status();
    printf("WIFI status: started=%d connected=%d got_ip=%d\n",
        st.started, st.connected, st.got_ip);
    printf("2 second Delay\n");
    vTaskDelay(pdMS_TO_TICKS(2000));
    printf("LVGL Demo Start...\n");
    lvgl_demo_start();
    printf("LVGL Finished\n");
}
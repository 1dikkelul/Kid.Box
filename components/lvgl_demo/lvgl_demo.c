#include "lvgl.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_types.h"
#include "esp_lcd_ili9341.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"

#include "../display/display.h"

#define LVGL_TICK_PERIOD_MS 5
#define LV_BUF_WIDTH 240
#define LV_BUF_HEIGHT 40
#define LV_SCREEN_WIDTH 240
#define LV_SCREEN_HEIGHT 320

static const char *TAG = "lvgl_demo";

// Global panel handle for flush callback
static esp_lcd_panel_handle_t panel_handle = NULL;

#define LCD_HOST    SPI2_HOST
#define PIN_NUM_MOSI 13
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   15
#define PIN_NUM_DC   2
#define PIN_NUM_RST  -1
#define TFT_BL 27  // Backlight pin

static void lv_tick_task(void *arg) {
    while (1) {
        lv_tick_inc(LVGL_TICK_PERIOD_MS);
        vTaskDelay(pdMS_TO_TICKS(LVGL_TICK_PERIOD_MS));
    }
}

static void my_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
    // Convert LVGL area to ILI9341 coordinates
    int x1 = area->x1;
    int y1 = area->y1;
    int x2 = area->x2;
    int y2 = area->y2;
    // esp_lcd_panel_draw_bitmap expects 16-bit color, so cast px_map
    esp_lcd_panel_draw_bitmap(panel_handle, x1, y1, x2 + 1, y2 + 1, (void *)px_map);
    lv_display_flush_ready(display);
}

void lvgl_demo_start(void) {
    lv_init();
    xTaskCreate(lv_tick_task, "lv_tick_task", 2048, NULL, 1, NULL);

    // Get the panel handle from display
    panel_handle = display_get_panel_handle();

    lv_color_t *buf1 = heap_caps_malloc(LV_BUF_WIDTH * LV_BUF_HEIGHT * sizeof(lv_color_t), MALLOC_CAP_DMA);
    if (!buf1) {
        ESP_LOGE(TAG, "Failed to allocate LVGL buffer");
        return;
    }

    lv_draw_buf_t draw_buf;
    lv_draw_buf_init(&draw_buf, LV_BUF_WIDTH, LV_BUF_HEIGHT, LV_COLOR_FORMAT_NATIVE, LV_BUF_WIDTH, buf1, LV_BUF_WIDTH * LV_BUF_HEIGHT * sizeof(lv_color_t));

    lv_display_t *display = lv_display_create(LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_display_set_buffers(display, buf1, NULL, LV_BUF_WIDTH * LV_BUF_HEIGHT * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, my_flush_cb);

    // Create a simple label
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_center(label);

    ESP_LOGI(TAG, "LVGL demo started");

    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

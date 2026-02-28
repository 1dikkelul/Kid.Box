idf_component_register(
    SRCS "app_main.c"
    INCLUDE_DIRS "."
    REQUIRES wifi_manager nvs_flash esp_lcd_ili9341 display lvgl_demo
)

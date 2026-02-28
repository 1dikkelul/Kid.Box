#ifndef DISPLAY_H
#define DISPLAY_H


#include "esp_lcd_types.h"
void display_init(void);
void display_test(void);
esp_lcd_panel_handle_t display_get_panel_handle(void);

#endif // DISPLAY_H

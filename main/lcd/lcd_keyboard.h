//
// Created by Stanislav Lakhtin on 13/03/2020.
//

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_LCD_KEYBOARD_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_LCD_KEYBOARD_H

#include "fonts.h"
#include "lcd.h"

static const char* SYMBOLS = "0123456789" "abcdefghijklmnopqrstuvwxyz" "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

typedef struct {
  char current;
  uint8_t position;
  uint16_t x0;
  uint16_t y0;
  uint16_t width;
  color_t * color;
  color_t * cmnd_color;
  color_t * bg_color;
} encoder_keyboard_t;

#define MAIN_SYMBOL_SIZE_X big
#define MAIN_SYMBOL_SIZE_Y big

#ifdef __cplusplus
extern "C"
{
#endif

esp_err_t draw_keyboard(lcd_device_t * dev, encoder_keyboard_t * keyboard);
esp_err_t move_left(lcd_device_t * dev, encoder_keyboard_t * keyboard);
esp_err_t move_right(lcd_device_t * dev, encoder_keyboard_t * keyboard);

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_LCD_KEYBOARD_H

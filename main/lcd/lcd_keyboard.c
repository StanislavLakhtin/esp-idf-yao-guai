/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI ILI-9340 G-Library

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.

    Support for several separate devices / screens in one library is antipattern.
    However, ST7789 and ILI-9340 have such a common interaction interface that combining them
    within a common library may make sense.

    This library is a compromise in this matter. The main goal is to ensure the speed of execution and
    the absence of checks on the time of execution.
*/

#include "lcd_keyboard.h"

esp_err_t draw_keyboard(lcd_device_t * dev, encoder_keyboard_t * kbrd) {
  // draw current symbol width / 2 - main_symbol_width / 2
  uint16_t main_symbol_width = kbrd->width / 2 - MAIN_SYMBOL_SIZE_X * 6;
  uint16_t x = kbrd->x0 + ( main_symbol_width / 2);
  draw_ascii_char_fast(dev, x, kbrd->y0, SYMBOLS[kbrd->position], kbrd->color, kbrd->bg_color, MAIN_SYMBOL_SIZE_X, MAIN_SYMBOL_SIZE_Y);
  return ESP_OK;
}


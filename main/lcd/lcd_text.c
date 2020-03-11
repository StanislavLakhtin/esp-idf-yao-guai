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

#include "lcd_driver.h"

// Standard / Cyrillic ASCII 5x7 font

static const unsigned char font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x55, 0x51, 0x55, 0x3E,
    0x3E, 0x6B, 0x6F, 0x6B, 0x3E,
    0x0C, 0x1E, 0x3C, 0x1E, 0x0C,
    0x08, 0x1C, 0x3E, 0x1C, 0x08,
    0x1C, 0x4A, 0x7F, 0x4A, 0x1C,
    0x18, 0x5C, 0x7F, 0x5C, 0x18,
    0x00, 0x1C, 0x1C, 0x1C, 0x00,
    0x7F, 0x63, 0x63, 0x63, 0x7F,
    0x00, 0x1C, 0x14, 0x1C, 0x00,
    0x7F, 0x63, 0x6B, 0x63, 0x7F,
    0x30, 0x48, 0x4D, 0x33, 0x07,
    0x06, 0x29, 0x79, 0x29, 0x06,
    0x20, 0x50, 0x3F, 0x02, 0x0C,
    0x60, 0x7F, 0x05, 0x35, 0x3F,
    0x2A, 0x1C, 0x77, 0x1C, 0x2A,
    0x00, 0x7F, 0x3E, 0x1C, 0x08,
    0x08, 0x1C, 0x3E, 0x7F, 0x00,
    0x14, 0x22, 0x7F, 0x22, 0x14,
    0x00, 0x5F, 0x00, 0x5F, 0x00,
    0x06, 0x09, 0x7F, 0x01, 0x7F,
    0x4A, 0x55, 0x55, 0x55, 0x29,
    0x60, 0x60, 0x60, 0x60, 0x60,
    0x54, 0x62, 0x7F, 0x62, 0x54,
    0x08, 0x04, 0x7E, 0x04, 0x08,
    0x08, 0x10, 0x3F, 0x10, 0x08,
    0x08, 0x08, 0x2A, 0x1C, 0x08,
    0x08, 0x1C, 0x2A, 0x08, 0x08,
    0x1C, 0x10, 0x10, 0x10, 0x10,
    0x1C, 0x3E, 0x08, 0x3E, 0x1C,
    0x30, 0x3C, 0x3F, 0x3C, 0x30,
    0x06, 0x1E, 0x7E, 0x1E, 0x06,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x5F, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x07, 0x00,
    0x14, 0x7F, 0x14, 0x7F, 0x14,
    0x24, 0x2A, 0x7F, 0x2A, 0x12,
    0x23, 0x13, 0x08, 0x64, 0x62,
    0x36, 0x49, 0x56, 0x20, 0x50,
    0x00, 0x00, 0x07, 0x00, 0x00,
    0x00, 0x1C, 0x22, 0x41, 0x00,
    0x00, 0x41, 0x22, 0x1C, 0x00,
    0x14, 0x08, 0x3E, 0x08, 0x14,
    0x08, 0x08, 0x3E, 0x08, 0x08,
    0x00, 0xA0, 0x60, 0x00, 0x00,
    0x08, 0x08, 0x08, 0x08, 0x08,
    0x00, 0x60, 0x60, 0x00, 0x00,
    0x20, 0x10, 0x08, 0x04, 0x02,
    0x3E, 0x51, 0x49, 0x45, 0x3E,
    0x44, 0x42, 0x7F, 0x40, 0x40,
    0x42, 0x61, 0x51, 0x49, 0x46,
    0x21, 0x41, 0x45, 0x4B, 0x31,
    0x18, 0x14, 0x12, 0x7F, 0x10,
    0x27, 0x45, 0x45, 0x45, 0x39,
    0x3C, 0x4A, 0x49, 0x49, 0x30,
    0x01, 0x71, 0x09, 0x05, 0x03,
    0x36, 0x49, 0x49, 0x49, 0x36,
    0x06, 0x49, 0x49, 0x29, 0x1E,
    0x00, 0x6C, 0x6C, 0x00, 0x00,
    0x00, 0xAC, 0x6C, 0x00, 0x00,
    0x08, 0x14, 0x22, 0x41, 0x00, // 60
    0x14, 0x14, 0x14, 0x14, 0x14,
    0x00, 0x41, 0x22, 0x14, 0x08,
    0x02, 0x01, 0x51, 0x09, 0x06,
    0x3E, 0x41, 0x5D, 0x55, 0x5E,
    0x7C, 0x12, 0x11, 0x12, 0x7C,
    0x7F, 0x49, 0x49, 0x49, 0x36,
    0x3E, 0x41, 0x41, 0x41, 0x22,
    0x7F, 0x41, 0x41, 0x22, 0x1C,
    0x7F, 0x49, 0x49, 0x49, 0x41, // 69 0x45 E
    0x7F, 0x09, 0x09, 0x09, 0x01,
    0x3E, 0x41, 0x49, 0x49, 0x7A,
    0x7F, 0x08, 0x08, 0x08, 0x7F,
    0x00, 0x41, 0x7F, 0x41, 0x00,
    0x20, 0x40, 0x41, 0x3F, 0x01,
    0x7F, 0x08, 0x14, 0x22, 0x41,
    0x7F, 0x40, 0x40, 0x40, 0x60,
    0x7F, 0x02, 0x0C, 0x02, 0x7F,
    0x7F, 0x04, 0x08, 0x10, 0x7F,
    0x3E, 0x41, 0x41, 0x41, 0x3E,
    0x7F, 0x09, 0x09, 0x09, 0x06,
    0x3E, 0x41, 0x51, 0x21, 0x5E,
    0x7F, 0x09, 0x19, 0x29, 0x46,
    0x46, 0x49, 0x49, 0x49, 0x31,
    0x03, 0x01, 0x7F, 0x01, 0x03,
    0x3F, 0x40, 0x40, 0x40, 0x3F,
    0x1F, 0x20, 0x40, 0x20, 0x1F,
    0x3F, 0x40, 0x3C, 0x40, 0x3F,
    0x63, 0x14, 0x08, 0x14, 0x63,
    0x07, 0x08, 0x70, 0x08, 0x07,
    0x61, 0x51, 0x49, 0x45, 0x43,
    0x00, 0x7F, 0x41, 0x41, 0x00,
    0x02, 0x04, 0x08, 0x10, 0x20,
    0x00, 0x41, 0x41, 0x7F, 0x00,
    0x04, 0x02, 0x01, 0x02, 0x04,
    0x40, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x01, 0x02, 0x04, 0x00,
    0x20, 0x54, 0x54, 0x54, 0x78,
    0x7F, 0x48, 0x44, 0x44, 0x38,
    0x38, 0x44, 0x44, 0x44, 0x48,
    0x38, 0x44, 0x44, 0x48, 0x7F,
    0x38, 0x54, 0x54, 0x54, 0x18,
    0x08, 0x7E, 0x09, 0x01, 0x02,
    0x08, 0x54, 0x54, 0x58, 0x3C,
    0x7F, 0x08, 0x04, 0x04, 0x78,
    0x00, 0x44, 0x7D, 0x40, 0x00,
    0x20, 0x40, 0x44, 0x3D, 0x00,
    0x7F, 0x10, 0x10, 0x28, 0x44,
    0x00, 0x41, 0x7F, 0x40, 0x00,
    0x7C, 0x04, 0x78, 0x04, 0x78,
    0x7C, 0x08, 0x04, 0x04, 0x78,
    0x38, 0x44, 0x44, 0x44, 0x38,
    0x7C, 0x14, 0x14, 0x14, 0x08,
    0x08, 0x14, 0x14, 0x0C, 0x7C,
    0x7C, 0x08, 0x04, 0x04, 0x08,
    0x48, 0x54, 0x54, 0x54, 0x24,
    0x04, 0x3F, 0x44, 0x40, 0x20,
    0x3C, 0x40, 0x40, 0x20, 0x7C,
    0x1C, 0x20, 0x40, 0x20, 0x1C,
    0x3C, 0x40, 0x38, 0x40, 0x3C,
    0x44, 0x28, 0x10, 0x28, 0x44,
    0x0C, 0x50, 0x50, 0x50, 0x3C,
    0x44, 0x64, 0x54, 0x4C, 0x44,
    0x00, 0x08, 0x36, 0x41, 0x00,
    0x00, 0x00, 0x7F, 0x00, 0x00,
    0x00, 0x41, 0x36, 0x08, 0x00,
    0x02, 0x01, 0x02, 0x04, 0x02,
    0x70, 0x48, 0x44, 0x48, 0x70,
    0x00, 0x0E, 0x11, 0x0E, 0x00,
    0x00, 0x12, 0x1F, 0x10, 0x00,
    0x00, 0x12, 0x19, 0x16, 0x00,
    0x00, 0x11, 0x15, 0x0B, 0x00,
    0x00, 0x07, 0x04, 0x1F, 0x00,
    0x00, 0x17, 0x15, 0x09, 0x00,
    0x00, 0x0E, 0x15, 0x09, 0x00,
    0x00, 0x01, 0x1D, 0x03, 0x00,
    0x00, 0x0A, 0x15, 0x0A, 0x00,
    0x00, 0x12, 0x15, 0x0E, 0x00,
    0x00, 0x04, 0x04, 0x04, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x3E, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x3E, 0x00, 0x00, 0x00,
    0x3E, 0x3E, 0x00, 0x3E, 0x00,
    0x3E, 0x3E, 0x00, 0x3E, 0x3E,
    0x58, 0x64, 0x04, 0x64, 0x58,
    0x7F, 0x3E, 0x1C, 0x08, 0x7F,
    0x7F, 0x08, 0x1C, 0x3E, 0x7F,
    0x7F, 0x7F, 0x00, 0x7F, 0x7F,
    0x08, 0x3E, 0x22, 0x22, 0x22,
    0x22, 0x22, 0x22, 0x3E, 0x08,
    0x40, 0x00, 0x40, 0x00, 0x40,
    0x60, 0x00, 0x40, 0x00, 0x40,
    0x60, 0x00, 0x70, 0x00, 0x40,
    0x60, 0x00, 0x70, 0x00, 0x78,
    0x7C, 0x00, 0x40, 0x00, 0x40,
    0x7C, 0x00, 0x7E, 0x00, 0x40,
    0x7C, 0x00, 0x7E, 0x00, 0x7F,
    0x1C, 0x77, 0x41, 0x41, 0x41,
    0x41, 0x41, 0x41, 0x41, 0x41,
    0x41, 0x41, 0x41, 0x7F, 0x00,
    0x1C, 0x77, 0x41, 0x5D, 0x5D,
    0x41, 0x41, 0x41, 0x5D, 0x5D,
    0x5D, 0x5D, 0x41, 0x5D, 0x5D,
    0x5D, 0x5D, 0x41, 0x7F, 0x00,
    0x22, 0x1C, 0x14, 0x1C, 0x22,
    0x00, 0x08, 0x1C, 0x08, 0x00,
    0x00, 0x00, 0x77, 0x00, 0x00,
    0x46, 0x5D, 0x55, 0x5D, 0x31,
    0x7C, 0x55, 0x54, 0x55, 0x44,
    0x08, 0x08, 0x2A, 0x08, 0x08,
    0x00, 0x14, 0x08, 0x14, 0x00,
    0x08, 0x14, 0x22, 0x08, 0x14,
    0x7F, 0x41, 0x71, 0x31, 0x1F,
    0x03, 0x05, 0x7F, 0x05, 0x03,
    0x22, 0x14, 0x7F, 0x55, 0x22,
    0x02, 0x55, 0x7D, 0x05, 0x02,
    0x06, 0x09, 0x09, 0x06, 0x00,
    0x44, 0x44, 0x5F, 0x44, 0x44,
    0x1C, 0x14, 0x1C, 0x22, 0x7F,
    0x20, 0x3E, 0x61, 0x3E, 0x20,
    0x20, 0x50, 0x3F, 0x02, 0x0C,
    0x80, 0x7C, 0x20, 0x3C, 0x40,
    0x44, 0x3C, 0x04, 0x7C, 0x44,
    0x00, 0x00, 0x08, 0x00, 0x00,
    0x38, 0x55, 0x54, 0x55, 0x18,
    0x7E, 0x08, 0x10, 0x7F, 0x01,
    0x08, 0x10, 0x08, 0x04, 0x02,
    0x14, 0x08, 0x22, 0x14, 0x08,
    0x0E, 0x06, 0x0A, 0x10, 0x20,
    0x20, 0x10, 0x0A, 0x06, 0x0E,
    0x38, 0x30, 0x28, 0x04, 0x02,
    0x02, 0x04, 0x28, 0x30, 0x38,
    0x7E, 0x11, 0x11, 0x11, 0x7E,
    0x7F, 0x49, 0x49, 0x49, 0x31,
    0x7F, 0x49, 0x49, 0x49, 0x36,
    0x7F, 0x01, 0x01, 0x01, 0x03,
    0xC0, 0x7F, 0x41, 0x7F, 0xC0,
    0x7F, 0x49, 0x49, 0x49, 0x41,
    0x77, 0x08, 0x7F, 0x08, 0x77,
    0x41, 0x49, 0x49, 0x49, 0x36,
    0x7F, 0x10, 0x08, 0x04, 0x7F,
    0x7C, 0x21, 0x12, 0x09, 0x7C,
    0x7F, 0x08, 0x14, 0x22, 0x41,
    0x40, 0x3E, 0x01, 0x01, 0x7F,
    0x7F, 0x02, 0x0C, 0x02, 0x7F,
    0x7F, 0x08, 0x08, 0x08, 0x7F,
    0x3E, 0x41, 0x41, 0x41, 0x3E,
    0x7F, 0x01, 0x01, 0x01, 0x7F,
    0x7F, 0x09, 0x09, 0x09, 0x06,
    0x3E, 0x41, 0x41, 0x41, 0x22,
    0x01, 0x01, 0x7F, 0x01, 0x01,
    0x07, 0x48, 0x48, 0x48, 0x3F,
    0x0E, 0x11, 0x7F, 0x11, 0x0E,
    0x63, 0x14, 0x08, 0x14, 0x63,
    0x7F, 0x40, 0x40, 0x7F, 0xC0,
    0x07, 0x08, 0x08, 0x08, 0x7F,
    0x7F, 0x40, 0x7F, 0x40, 0x7F,
    0x7F, 0x40, 0x7F, 0x40, 0xFF,
    0x01, 0x7F, 0x48, 0x48, 0x30,
    0x7F, 0x48, 0x48, 0x30, 0x7F,
    0x7F, 0x48, 0x48, 0x48, 0x30,
    0x22, 0x41, 0x49, 0x49, 0x3E,
    0x7F, 0x08, 0x3E, 0x41, 0x3E,
    0x46, 0x29, 0x19, 0x09, 0x7F,
    0x20, 0x54, 0x54, 0x54, 0x78,
    0x3C, 0x4A, 0x4A, 0x49, 0x31,
    0x7C, 0x54, 0x54, 0x54, 0x28,
    0x7C, 0x04, 0x04, 0x04, 0x0C,
    0xC0, 0x78, 0x44, 0x7C, 0xC0,
    0x38, 0x54, 0x54, 0x54, 0x18,
    0x6C, 0x10, 0x7C, 0x10, 0x6C,
    0x44, 0x54, 0x54, 0x54, 0x28,
    0x7C, 0x20, 0x10, 0x08, 0x7C,
    0x7C, 0x40, 0x26, 0x10, 0x7C,
    0x7C, 0x10, 0x10, 0x28, 0x44,
    0x40, 0x38, 0x04, 0x04, 0x7C,
    0x7C, 0x08, 0x10, 0x08, 0x7C,
    0x7C, 0x10, 0x10, 0x10, 0x7C,
    0x38, 0x44, 0x44, 0x44, 0x38,
    0x7C, 0x04, 0x04, 0x04, 0x7C,
    0x7C, 0x14, 0x14, 0x14, 0x08,
    0x38, 0x44, 0x44, 0x44, 0x48,
    0x04, 0x04, 0x7C, 0x04, 0x04,
    0x0C, 0x50, 0x50, 0x50, 0x3C,
    0x18, 0x24, 0xFC, 0x24, 0x18,
    0x44, 0x28, 0x10, 0x28, 0x44,
    0x7C, 0x40, 0x40, 0x7C, 0xC0,
    0x0C, 0x10, 0x10, 0x10, 0x7C,
    0x7C, 0x40, 0x7C, 0x40, 0x7C,
    0x7C, 0x40, 0x7C, 0x40, 0xFC,
    0x04, 0x7C, 0x50, 0x50, 0x20,
    0x7C, 0x50, 0x50, 0x20, 0x7C,
    0x7C, 0x50, 0x50, 0x50, 0x20, //
    0x28, 0x44, 0x54, 0x54, 0x38, // 0xfd ь
    0x7C, 0x10, 0x38, 0x44, 0x38, // 0xfe э
    0x48, 0x34, 0x14, 0x14, 0x7C  // 0xff ю
};

// based on Adafruit GFX lib. It's works well but is deprecated because too slow
/// deprecated
esp_err_t draw_ascii_char(lcd_device_t *dev, int16_t x, int16_t y, unsigned char c,
                          color_t * text_color, color_t * bg_color, ascii_font_size_t size_x,
                          ascii_font_size_t size_y) {
  if ((x >= dev->width) || (y >= dev->height))
    return ESP_OK;

  for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
    uint8_t line = font[c * 5 + i];
    for (int8_t j = 0; j < 8; j++, line >>= 1) {
      if (line & 1) {
        if (size_x == 1 && size_y == 1)
          draw_pixel(dev, x + i, y + j, text_color);
        else
          fill_rect(dev, x + i * size_x, y + j * size_y, size_x, size_y, text_color);
      } else if (is_colors_equal (bg_color, text_color)) {
        if (size_x == 1 && size_y == 1)
          draw_pixel(dev, x + i, y + j, bg_color);
        else
          fill_rect(dev, x + i * size_x, y + j * size_y, size_x, size_y, bg_color);
      }
    }
  }
  if (is_colors_equal (bg_color, text_color)) { // If opaque, draw vertical line for last column
    if (size_x == 1 && size_y == 1)
      draw_fast_v_line(dev, x + 5, y, 8, bg_color);
    else
      fill_rect(dev, x + 5 * size_x, y, size_x, 8 * size_y, bg_color);
  }
  return ESP_OK;
}

esp_err_t draw_ascii_char_fast(lcd_device_t *dev, int16_t x, int16_t y, unsigned char c,
                               color_t * text_color, color_t * bg_color, ascii_font_size_t size_x,
                               ascii_font_size_t size_y) {
#ifdef AUTO_CHANGE_TEXT_X_Y_DIMENSION_SIZES
  swap(ascii_font_size_t, size_x, size_y)
#endif
  if (((x + 6 * size_x) >= dev->width)
      || ((y + 8 * size_y) >= dev->height))
    return ESP_OK;

  if (size_x > 4 || size_y > 4 || size_x < 1 || size_y < 1)  // Wrong array size
    return ESP_OK;

  uint16_t pixel_size = size_x * size_y;
  uint32_t pixels = 6 * 8 * pixel_size;
  uint8_t bytes_per_pixel = 2;
  uint8_t *char_buffer = malloc(bytes_per_pixel * pixels);
  uint8_t double_size_x = 2 * size_x;
  if (char_buffer == NULL)
    return ESP_OK;

  for (int8_t current_x = 0; current_x < 5; current_x++) { // read by columns: 5x columns with data and space 1x column
    uint8_t line = font[c * 5 + current_x];
    for (int8_t current_y = 0; current_y < 8; current_y++, line >>= 1) {
      uint32_t base_xline_offset = 6 * pixel_size * current_y + current_x * size_x;
      for (uint8_t line_y = 0; line_y < size_y; line_y++) {
        uint32_t array_pos_y = base_xline_offset + 6 * size_x * line_y;
        if (line & 1) {
          color_list_to_bytes(char_buffer + array_pos_y * bytes_per_pixel, double_size_x, text_color, size_x);
        } else
          color_list_to_bytes(char_buffer + array_pos_y * bytes_per_pixel, double_size_x, bg_color, size_x);
      }
    }
  }

  // dirty hack -- draw backgound on empty column
  uint32_t base_xline_offset = 5 * size_x * bytes_per_pixel;
  uint32_t line_length = 6 * double_size_x;
  for (int line_y = 0; line_y < 8 * size_y; line_y++) {
    uint32_t  offset = line_length * line_y + base_xline_offset;
    color_list_to_bytes(char_buffer + offset, double_size_x, bg_color, size_x);
  }
  ESP_ERROR_CHECK_WITH_INTERRUPT(frame_fill(dev, x, x + 6 * size_x - 1, y, y + 8 * size_y - 1, char_buffer, bytes_per_pixel * pixels, 1));
  free(char_buffer);
  return ESP_OK;
}

esp_err_t draw_monospace_text(lcd_device_t *dev, ascii_text_frame_t *text_frame, const char *text, size_t max_length,
                              color_t * color, color_t * bg_color, ascii_font_size_t size_x, ascii_font_size_t size_y) {
  uint16_t index = 0;
  size_t str_len = strlen(text);
  str_len = (str_len < max_length) ? str_len : max_length;
  char *dest = malloc(str_len);
  if (dest == NULL)
    return ESP_OK;
  utf8_to_extended_ascii_codes(dest, text, str_len);
  str_len = strlen(dest);
  text_frame->current_x = max(text_frame->current_x ,text_frame->x0);
  text_frame->current_y = max(text_frame->current_y , text_frame->y0);
  while (index < str_len) {
    if ((text_frame->current_x + size_x * 6 > text_frame->x1) ||
        (text_frame->current_y + text_frame->row_height * size_y > text_frame->y1))
      return ESP_OK;
    if (dest[index] == '\n') {
      text_frame->current_x = text_frame->x0;
      text_frame->current_y += text_frame->row_height * size_y;
    } else if ( dest[index] == '\r' ){
      text_frame->current_x = text_frame->x0;
    } else if ( dest[index] == '\t' ){
      text_frame->current_x += size_x * 6 * 2;
    } else {
      draw_ascii_char_fast(dev, text_frame->current_x, text_frame->current_y, dest[index], color, bg_color, size_x, size_y);
      text_frame->current_x += 6 * size_x;
      if (text_frame->auto_wrap && (text_frame->current_x + size_x * 6) >= text_frame->x1) {
        text_frame->current_x = text_frame->x0;
        text_frame->current_y += text_frame->row_height * size_y;
      }
    }
    index += 1;
  }
  free(dest);
  return ESP_OK;
}
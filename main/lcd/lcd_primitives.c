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

#include "lcd.h"

esp_err_t draw_pixel(lcd_device_t *dev, uint16_t x, uint16_t y, color_t * color) {
  if (x >= dev->width || y >= dev->height)
    return ESP_OK;
  uint8_t buf[2];
  color_list_to_bytes(buf, 2, color, 1);
  ESP_ERROR_CHECK_WITH_INTERRUPT(frame_set(dev, x, x, y, y));
  ESP_ERROR_CHECK_WITH_INTERRUPT(dev->write_cmnd(RAMWR));
  return (dev->write_data(buf, 2));
}

esp_err_t draw_circle(lcd_device_t *dev, int16_t x0, int16_t y0, int16_t r, color_t * color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  draw_pixel(dev, x0, y0 + r, color);
  draw_pixel(dev, x0, y0 - r, color);
  draw_pixel(dev, x0 + r, y0, color);
  draw_pixel(dev, x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    draw_pixel(dev, x0 + x, y0 + y, color);
    draw_pixel(dev, x0 - x, y0 + y, color);
    draw_pixel(dev, x0 + x, y0 - y, color);
    draw_pixel(dev, x0 - x, y0 - y, color);
    draw_pixel(dev, x0 + y, y0 + x, color);
    draw_pixel(dev, x0 - y, y0 + x, color);
    draw_pixel(dev, x0 + y, y0 - x, color);
    draw_pixel(dev, x0 - y, y0 - x, color);
  }
  return ESP_OK;
}

esp_err_t draw_fast_v_line(lcd_device_t *dev, uint16_t x0, uint16_t y0, uint16_t h, color_t * color) {
  uint16_t _h = y0 + h;
  _h = (_h < dev->height) ? _h : (dev->height - y0);
  if (x0 > dev->width || y0 > dev->height)
    return ESP_OK;
  uint32_t _buf_len = get_color_buf_depth(_h);
  uint8_t buf[_buf_len];
  color_list_to_bytes(buf, _buf_len, color, _h);
  ESP_ERROR_CHECK_WITH_INTERRUPT(frame_fill(dev, x0, x0, y0, _h, buf, _buf_len, 1));
  return ESP_OK;
}

esp_err_t draw_fast_h_line(lcd_device_t *dev, uint16_t x0, uint16_t y0, uint16_t w, color_t * color) {
  uint16_t _w = x0 + w;
  _w = (_w < dev->width) ? _w : (dev->width - x0);
  if (x0 > dev->width || y0 > dev->height)
    return ESP_OK;
  uint32_t _bl = get_color_buf_depth(_w);
  uint8_t buf[_bl];
  color_list_to_bytes(buf, _bl, color, _w);
  ESP_ERROR_CHECK_WITH_INTERRUPT(frame_fill(dev, x0, _w, y0, y0, buf, _bl, 1));
  return ESP_OK;
}

#define FILL_BUFFER_LENGTH 120
esp_err_t fill_rect(lcd_device_t *dev, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, color_t * color) {

  uint16_t y1 = ( y0 + height > dev->height) ? dev->height : y0 + height;
  uint16_t x1 = ( x0 + width > dev->width) ? dev->width : x0 + width;
  uint32_t pixel_length = (x1 - x0) * (y1 - y0);
  uint8_t buffer[FILL_BUFFER_LENGTH];
  uint16_t half = FILL_BUFFER_LENGTH / 2;
  color_list_to_bytes(buffer, FILL_BUFFER_LENGTH, color, half);

  ESP_ERROR_CHECK_WITH_INTERRUPT(frame_set(dev, x0, x1, y0, y1));
  ESP_ERROR_CHECK_WITH_INTERRUPT(dev->write_cmnd(RAMWR));
  uint32_t i = 0;
  while (i < pixel_length) {
    uint32_t pixel_to_send = (pixel_length - i < half) ? pixel_length - i : half;
    ESP_ERROR_CHECK_WITH_INTERRUPT(dev->write_data(buffer, pixel_to_send * 2));
    i += pixel_to_send;
  }
  return ESP_OK;
}

esp_err_t draw_fast_h_gradient_line(lcd_device_t *dev, uint16_t x0, uint16_t y0, uint16_t w, color_t * begin_color,
                                    color_t * end_color) {
  if (x0 > dev->width || y0 > dev->height)
    return ESP_OK;
  uint16_t _w = x0 + w;
  _w = (_w < dev->width) ? _w : dev->width;
  uint32_t _bl = get_color_buf_depth(_w);
  uint8_t buf[_bl];
  color_list_to_gradient_bytes(buf, _bl, _w, begin_color, end_color);
  ESP_ERROR_CHECK_WITH_INTERRUPT(frame_fill(dev, x0, _w, y0, y0, buf, _bl, 1));
  return ESP_OK;
}

esp_err_t draw_fast_rect(lcd_device_t *dev, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t border_width, color_t * color) {
  if ( x1 < x0 )
    swap(uint16_t, x1, x0)
  if ( y1 < y0 )
    swap(uint16_t, y1, y0)
  uint16_t w = x1 - x0;
  uint16_t h = y1 - y0;
  uint16_t delta = 0;
  for (int i=0; i < border_width; i++) {
    if ( ( x1 - delta ) > ( x0 + delta ) ) {
      draw_fast_v_line(dev, x0 + delta, y0, h, color);
      draw_fast_v_line(dev, x1 - delta, y0, h, color);
    }
    if ( ( y1 - delta ) > ( y0 + delta ) ) {
      draw_fast_h_line(dev, x0, y0 + delta, w, color);
      draw_fast_h_line(dev, x0, y1 - delta, w, color);
    }
    delta += 1;
  }
  return ESP_OK;
}

esp_err_t draw_fast_rect_fill(lcd_device_t *dev, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t border_width, color_t * color, color_t * bg_color) {
  if ( x1 < x0 )
  swap(uint16_t, x1, x0)
  if ( y1 < y0 )
  swap(uint16_t, y1, y0)
  uint16_t w = x1 - x0;
  uint16_t h = y1 - y0;
  fill_rect(dev, x0, y0, w, h, bg_color);
  draw_fast_rect(dev, x0, y0, x1, y1, border_width, color);
  return ESP_OK;
}
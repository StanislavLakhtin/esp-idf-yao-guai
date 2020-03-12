#include "lcd_driver.h"

esp_err_t draw_button(lcd_device_t *dev, ascii_text_frame_t *frame, const char * text, size_t max_text_length,
                      color_t * color, color_t * bg_color, color_t * brdr_color,uint8_t border_sz, uint16_t padding,
                      ascii_font_size_t size_x, ascii_font_size_t size_y) {
  draw_fast_rect_fill(dev, frame->x0, frame->y0, frame->x1, frame->y1, border_sz, brdr_color, bg_color);
  size_t str_len = strlen(text);
  str_len = (str_len < max_text_length) ? str_len : max_text_length;
  ascii_text_frame_t txt_frame = {
      .x0 = frame->x0 + padding + border_sz,
      .y0 = frame->y0 + padding + border_sz + 1,   // empty line above symbols
      .x1 = frame->x1 - padding - border_sz,
      .y1 = frame->y1 - padding - border_sz,
      .auto_wrap = frame->auto_wrap,
      .row_height = DEFAULT_ROW_HEIGHT,
  };
  uint16_t x_width = (txt_frame.x1 - txt_frame.x0);
  uint16_t x_str_width = str_len * size_x * 6;
  if ( x_width > x_str_width ) {
    //printf("%d, %d, %d", txt_frame.x0, x_width, x_str_width);
    txt_frame.current_x = txt_frame.x0 + (x_width - x_str_width) / 2;
  }
  draw_monospace_text(dev, &txt_frame, text, max_text_length, color, bg_color, size_x, size_y);
  return ESP_OK;
}

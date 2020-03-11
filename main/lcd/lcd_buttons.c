#include "lcd_driver.h"

esp_err_t draw_button(lcd_device_t *dev, ascii_text_frame_t *frame, const char * text, size_t max_text_length,
                      color_t * color, color_t * bg_color, color_t * brdr_color, uint16_t padding,
                      ascii_font_size_t size_x, ascii_font_size_t size_y) {
  printf("draw button. fill rect (%d,%d)-(%d,%d)", frame->x0, frame->y0, frame->x1,  frame->y1);
  fill_rect(dev, frame->x0, frame->y0, frame->x1,  frame->y1, bg_color);
  uint16_t w = frame->x1-frame->x0;
  draw_fast_h_line(dev, frame->x0, frame->y0, w, brdr_color);
  draw_fast_h_line(dev, frame->x1, frame->y0, w, brdr_color);
  uint16_t h = frame->y1-frame->y0;
  draw_fast_h_line(dev, frame->x0, frame->y0, h, brdr_color);
  draw_fast_h_line(dev, frame->x1, frame->y0, h, brdr_color);
  ascii_text_frame_t txt_frame = {
      .x0 = frame->x0 + padding,
      .y0 = frame->y0 + padding,
      .x1 = frame->x1 - padding,
      .y1 = frame->y1 - padding,
      .auto_wrap = frame->auto_wrap,
      .row_height = DEFAULT_ROW_HEIGHT,
  };
  draw_monospace_text(dev, &txt_frame, text, max_text_length, color, bg_color, size_x, size_y);
  return ESP_OK;
}

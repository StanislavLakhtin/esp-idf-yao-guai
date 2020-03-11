#include "lcd_driver.h"

esp_err_t draw_button(lcd_device_t *dev, ascii_text_frame_t *frame, const char * text, size_t max_text_length,
                      color_t * color, color_t * bg_color, color_t * brdr_color,uint8_t border_sz, uint16_t padding,
                      ascii_font_size_t size_x, ascii_font_size_t size_y) {
  uint16_t w = frame->x1 - frame->x0;
  uint16_t h = frame->y1 - frame->y0;
  fill_rect(dev, frame->x0, frame->y0, w , h, bg_color);
  uint8_t delta = 0;
  for (int i = 0 ; i < border_sz; i++) {
    draw_fast_v_line(dev, frame->x0 + delta, frame->y0, h, brdr_color);
    draw_fast_v_line(dev, frame->x1 - delta, frame->y0, h, brdr_color);
    draw_fast_h_line(dev, frame->x0, frame->y0 + delta, w, brdr_color);
    draw_fast_h_line(dev, frame->x0, frame->y1 - delta, w, brdr_color);
    delta += 1;
  }
  ascii_text_frame_t txt_frame = {
      .x0 = frame->x0 + padding + delta,
      .y0 = frame->y0 + padding + delta,
      .x1 = frame->x1 - padding - delta,
      .y1 = frame->y1 - padding - delta,
      .auto_wrap = frame->auto_wrap,
      .row_height = DEFAULT_ROW_HEIGHT,
  };
  printf("\nbutton frame (%d,%d)-(%d,%d)", frame->x0, frame->y0, frame->x1,  frame->y1);
  printf("\nbutton text frame (%d,%d)-(%d,%d)", txt_frame.x0, txt_frame.y0, txt_frame.x1,  txt_frame.y1);
  draw_monospace_text(dev, &txt_frame, text, max_text_length, color, bg_color, size_x, size_y);
  return ESP_OK;
}

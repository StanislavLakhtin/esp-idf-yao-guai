//
// Created by Stanislav Lakhtin on 06/03/2020.
//

#include "ui_fsm.h"

static const char * hello = "YaoGuai WS\r";

enum ret_codes_t ui_idle_state( void ) {
  ESP_LOGI(TAG, "Process to UI idle state");
  color_t bg , c;
  set_color(bg, 0,0,0);
  fill_rect(lcd_dev, 0, 0, lcd_dev->width,  lcd_dev->height, &bg);
  ascii_text_frame_t ascii_frame = {
      .x0 = 4,
      .y0 = 4,
      .x1 = lcd_dev->width - 4,
      .y1 = lcd_dev->height -4,
      .auto_wrap = true,
      .row_height = DEFAULT_ROW_HEIGHT,
      .current_x = 4,
      .current_y = 4
  };
  for (uint8_t i = 0; i < MAX_INTENSITY; i++ ) {
    set_color(c, i,i,i);
    draw_monospace_text(lcd_dev, &ascii_frame, hello, strlen(hello), &c, &bg, normal, normal);
  }
  LISTEN_IO_MS(1000);
  return FSM_OK ;
}


void ui0_listener( btns_event_t event ) {
  printf("event: %c", event);
}
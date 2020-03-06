//
// Created by Stanislav Lakhtin on 06/03/2020.
//

#include "ui_fsm.h"

static const char * hello = "YaoGuai WS";

enum ret_codes_t ui_init_state( void ) {
  ESP_LOGI(TAG, "Process to UI init state");
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
  loop {
    for (uint8_t i = 0; i < MAX_INTENSITY; i++ ) {
      set_color(c, i,i,i);
      printf("%d/", i);
      draw_monospace_text(lcd_dev, &ascii_frame, hello, strlen(hello), &c, &bg, normal, normal);
    }
    uint32_t ulNotifiedValue = 0x00;
    //xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, 10);
    if (ulNotifiedValue) {
      return FSM_OK;
    }
    PAUSE(1);
  }
  return FSM_REPEAT ;
}
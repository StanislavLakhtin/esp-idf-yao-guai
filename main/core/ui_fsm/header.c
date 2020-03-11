/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI UI FSM

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

#include "ui_fsm.h"

void yg_draw_header( void ) {
  char wifi_str[40];
  ESP_LOGI( TAG, "header" );
  color_t bg, c;
  set_color( bg, 30, 40, 0 );
  set_color( c, 0, 0, 0 );
  fill_rect( lcd_dev, 0, 0, lcd_dev->width, 18, &bg );
  ascii_text_frame_t ascii_frame = {
      .x0 = 1,
      .y0 = 1,
      .x1 = lcd_dev->width - 1,
      .y1 = 17,
      .auto_wrap = true,
      .row_height = DEFAULT_ROW_HEIGHT,
  };
  printf("x0: %d, y0: %d, x1: %d, y1: %d, x: %d, y: %d", ascii_frame.x0, ascii_frame.y0,
         ascii_frame.x1, ascii_frame.y1, ascii_frame.current_x, ascii_frame.current_y);
  memset(wifi_str, 0x00, 40);
  strcpy(wifi_str, "WiFi: ");
  draw_monospace_text( lcd_dev, &ascii_frame, wifi_str, strlen( wifi_str ), &c, &bg, tiny, normal );
}


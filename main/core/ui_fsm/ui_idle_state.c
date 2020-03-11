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

static const char * hello_str = "YaoGuai WS\r";
typedef enum {
  hello_screen = 0,
  screen0 = 1
} display_state_t;

static display_state_t st;
enum ret_codes_t r_code;

static void draw_hello_str( void ) {
  ESP_LOGI( TAG, "Hello screen" );
  color_t bg, c;
  set_color( bg, 0, 0, 0 );
  fill_rect( lcd_dev, 0, 0, lcd_dev->width, lcd_dev->height, &bg );
  ascii_text_frame_t ascii_frame = {
      .x0 = 4,
      .y0 = 4,
      .x1 = lcd_dev->width - 4,
      .y1 = lcd_dev->height - 4,
      .auto_wrap = true,
      .row_height = DEFAULT_ROW_HEIGHT,
      .current_x = 4,
      .current_y = 4
  };
  for ( uint8_t i = 0 ; i < MAX_INTENSITY ; i++ ) {
    set_color( c, i, i, i );
    draw_monospace_text( lcd_dev, &ascii_frame, hello_str, strlen( hello_str ), &c, &bg, normal, normal );
    delayMs( 10 );
  }
}

static void draw_screen0( void ) {
  ESP_LOGI( TAG, "Screen0" );
  LISTEN_IO_MS(1000);
}

void ui0_listener( btns_event_t event ) {
  printf( "event: %c", event );
  r_code = FSM_OK;
}

enum ret_codes_t ui_idle_view( void ) {
  ESP_LOGI( TAG, "UI idle state" );
  st = hello_screen;
  r_code = FSM_REPEAT;
  do {
    switch (st) {
      default:
      case hello_screen:
        draw_hello_str(); st = screen0; break;
      case screen0:
        draw_screen0(); break;
    }
  } while (r_code == FSM_REPEAT);
  return r_code;
}
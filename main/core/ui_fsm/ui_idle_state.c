//
// Created by Stanislav Lakhtin on 06/03/2020.
//

#include "ui_fsm.h"

static const char * hello_str = "YaoGuai WS\r";
typedef enum {
  hello_screen = 0,
  screen0 = 1
} display_state_t;

static display_state_t st;
enum ret_codes_t r_code;

static void draw_hello_str( void ) {
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
  LISTEN_IO_MS(1000);
}

void ui0_listener( btns_event_t event ) {
  printf( "event: %c", event );
  r_code = FSM_OK;
}

enum ret_codes_t ui_idle_state( void ) {
  ESP_LOGI( TAG, "Process to UI idle state" );
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
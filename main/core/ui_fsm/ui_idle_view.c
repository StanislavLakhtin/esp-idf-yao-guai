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

static const char *hello_str = "YaoGuai WS\r";

enum ret_codes_t r_code;

static void draw_hello_str(void) {
  ESP_LOGI(TAG, "Hello screen");
  color_t bg, c;
  set_color(bg, 0, 0, 0);
  fill_rect(lcd_dev, 0, 0, lcd_dev->width, lcd_dev->height, &bg);
  ascii_text_frame_t ascii_frame = {
      .x0 = lcd_dev->width / 2 - strlen(hello_str) * 6 - 4,
      .y0 = lcd_dev->height / 2 - 28,
      .x1 = lcd_dev->width - 4,
      .y1 = lcd_dev->height - 4,
      .auto_wrap = false,
      .row_height = DEFAULT_ROW_HEIGHT
  };
  printf("x0: %d, y0: %d, x1: %d, y1: %d, x: %d, y: %d", ascii_frame.x0, ascii_frame.y0,
         ascii_frame.x1, ascii_frame.y1, ascii_frame.current_x, ascii_frame.current_y);
  for (uint8_t i = 0; i < MAX_INTENSITY; i++) {
    set_color(c, i >> 1, i, i >> 1);
    draw_monospace_text(lcd_dev, &ascii_frame, hello_str, strlen(hello_str), &c, &bg, enormous, big);
    delayMs(10);
  }
}

static void draw_screen0(void) {
  ESP_LOGI(TAG, "Screen0");
  EventBits_t state_changes = xEventGroupGetBits(xEvents);
  if (state_changes & BIT_WIFI_CHANGE_STATE) {
    yg_draw_header();
    xEventGroupClearBits(xEvents, BIT_WIFI_CHANGE_STATE);
  } else
    printf("no wifi data change");
}

static screen_view_fptr_t view_method;
static uint8_t method_indx = 0x00;

#define SCREEN_CNT 3
static const screen_view_fptr_t screens[SCREEN_CNT] = {draw_screen0, draw_hello_str, yg_lcd_demo_view};

void ui0_listener(btns_event_t event) {
  printf("event: %c", event);
  switch (event) {
    case ENCODER0_PRESS:
      r_code = FSM_OK;
      break;
    case ENCODER0_ROTATE_RIGHT:
      method_indx = (method_indx >= SCREEN_CNT - 1) ? method_indx = 0: method_indx + 1;
      break;
    case ENCODER0_ROTATE_LEFT:
      method_indx = (method_indx == 0 ) ? method_indx = SCREEN_CNT - 1: method_indx - 1;
      break;
    default:
      break;
  }
  printf("method index: %d", method_indx);
}

enum ret_codes_t ui_idle_view(void) {
  ESP_LOGI(TAG, "UI idle state");
  r_code = FSM_REPEAT;
  do {
    if (method_indx < SCREEN_CNT)
      view_method = screens[method_indx];
    view_method();
    LISTEN_IO_MS(30000);
  } while (r_code == FSM_REPEAT);
  return r_code;
}
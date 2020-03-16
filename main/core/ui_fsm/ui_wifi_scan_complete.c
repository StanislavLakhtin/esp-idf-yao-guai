/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI UI Scan complete screen

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


static uint8_t scrn_indx = 0x00;
static uint8_t menu_indx = 0x00;

static enum ret_codes_t r_code = FSM_REPEAT;

static screen_view_fptr_t view_method;

static size_t max_sym_cnt() {
  return 32;
}

static void select_ap(void) {
  /*ascii_font_size_t header_size_x = normal;
  ascii_font_size_t header_size_y = normal;
  uint16_t border_sz = 2;
  uint16_t padding = 4;
  uint16_t margin = 8;
  uint16_t delta_x = lcd_dev->width / 2 - header_size_x * 6 * (max_sym_cnt() / 2) - border_sz * 2 - padding * 2;
  uint16_t btn_height =
      header_size_y * DEFAULT_ROW_HEIGHT + padding * 2 + border_sz * 2 + 1;    // 1 -- empty line above symbols
  uint16_t y_offset = (lcd_dev->height - btn_height * ap_cnt - margin * ap_cnt) / 2;
  color_t c, bg, brdr_clr;
  set_color(c, 0xff, 0xff, 0xff);
  set_color(brdr_clr, 0x33, 0x00, 0x00);
  for (int i = 0; i < ap_cnt; i++) {
    ascii_text_frame_t btn_frame = {
        .x0 = delta_x,
        .y0 = y_offset,
        .x1 = lcd_dev->width - delta_x,
        .y1 = y_offset + btn_height,
        .auto_wrap = false,
        .row_height = DEFAULT_ROW_HEIGHT,
    };
    if (i == menu_indx)
      set_color(bg, 55, 0x55, 0x55);
    else
      set_color(bg, 0xaa, 0xaa, 0xaa);
    const char * button_name = (const char *) ap_info[i].ssid;
    draw_button(lcd_dev, &btn_frame, button_name, strlen(button_name), &c, &bg, &brdr_clr, border_sz, padding,
                header_size_x, header_size_y);
    y_offset += btn_height + margin;
  }*/
  ESP_LOGI(TAG, "%d ap station should draw", ap_cnt);
}

void ui_wifi_scan_complete_listener(btns_event_t event) {
  ESP_LOGI(TAG, "%c", event);
}

#define SCREEN_CNT 1
static const screen_view_fptr_t screens[SCREEN_CNT] = {select_ap};

enum ret_codes_t ui_settings_scan_complete(void) {
  uint8_t crnt_scrn_indx = 255;
  do {
    if (scrn_indx != crnt_scrn_indx) {
      yg_draw_settings_menu_bg();
      crnt_scrn_indx = scrn_indx;
    }
    view_method = screens[scrn_indx];
    if (view_method != NULL)
      view_method();
    LISTEN_IO_MS(300000);
  } while (r_code == FSM_REPEAT);
  return r_code;
}
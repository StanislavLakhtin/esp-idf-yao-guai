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

static enum ret_codes_t r_code = FSM_REPEAT;

static screen_view_fptr_t view_method;

void ui_wifi_scan_complete_listener( btns_event_t event ) {
  ESP_LOGI(TAG, "%c", event);
}

enum ret_codes_t ui_settings_scan_complete( void ) {
  uint8_t crnt_scrn_indx = 255;
  do {
    if (scrn_indx != crnt_scrn_indx) {
      yg_draw_settings_menu_bg();
      crnt_scrn_indx = scrn_indx;
    }
    //view_method = screens[scrn_indx];
    if (view_method != NULL)
      view_method();
    LISTEN_IO_MS(300000);
  } while (r_code == FSM_REPEAT);
  return r_code;
}
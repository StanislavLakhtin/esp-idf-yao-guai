#pragma once
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

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_FSM_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_FSM_H

#include "../fsm.h"
#include "lcd/lcd_driver.h"
#include "lcd_spi_driver/esp32_lcd_spi_driver.h"

typedef enum  { // MUST BE THE SAME FSM ORDER as in ui loop task
  error = 0,
  idle = 1,
  settings = 2,
} ui_states_t;

typedef void (* screen_view_fptr_t )(void);
typedef void (* io_event_listener_fptr_t) ( btns_event_t event );

typedef struct {
  ui_states_t src;
  enum ret_codes_t ret_codes;
  ui_states_t dst;
} ui_transition_t;

#define UI_ENTRY_STATE idle
#define UI_UNKNOWN_STATE error

io_event_listener_fptr_t current_input_handler;

static const char * TAG = "ui";

#define LISTEN_IO_MS( xTimeInMs ) do { \
  btns_event_t event; \
    if (xQueueReceive(kbrd_evnt_queue, &event, pdMS_TO_TICKS ( xTimeInMs ))) { \
      if (current_input_handler != NULL )  {\
        current_input_handler(event); \
      } else ESP_LOGI( TAG, "NULL pointer input handler"); \
    } \
  } while (0);

lcd_device_t * lcd_dev;

#ifdef __cplusplus
extern "C"
{
#endif

enum ret_codes_t ui_idle_view( void );
enum ret_codes_t ui_error_view( void );
enum ret_codes_t ui_settings_view( void );

ui_states_t ui_lookup_transitions(ui_states_t state, enum ret_codes_t code );

void ui0_listener( btns_event_t event );
void menu_input_listener( btns_event_t event );

void yg_draw_header( void );
void yg_lcd_demo_view( void );

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_FSM_H

#pragma once
//
// Created by Stanislav Lakhtin on 04.03.2020.
//

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
    if(xQueueReceive(kbrd_evnt_queue, &event, pdMS_TO_TICKS ( xTimeInMs * 1000 ))) { \
      if (current_input_handler) current_input_handler(event); \
    } \
  } while (0);

lcd_device_t * lcd_dev;

#ifdef __cplusplus
extern "C"
{
#endif

enum ret_codes_t ui_idle_state( void );
enum ret_codes_t ui_error_state( void );
enum ret_codes_t ui_settings_state( void );

ui_states_t ui_lookup_transitions(ui_states_t state, enum ret_codes_t code );

void ui0_listener( btns_event_t event );

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_FSM_H

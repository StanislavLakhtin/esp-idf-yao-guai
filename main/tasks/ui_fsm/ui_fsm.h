#pragma once
//
// Created by Stanislav Lakhtin on 04.03.2020.
//

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_FSM_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_FSM_H

#include "../fsm.h"
#include "lcd/lcd_driver.h"
#include "lcd_spi_driver/esp32_lcd_spi_driver.h"

enum ui_states_t { // MUST BE THE SAME FSM ORDER as in ui loop task
  error = 0,
  init = 1,
  display = 2,
};

typedef struct {
  enum ui_states_t src;
  enum ret_codes_t ret_codes;
  enum ui_states_t dst;
} ui_transition_t;

#define UI_ENTRY_STATE init
#define UI_UNKNOWN_STATE display

static const char * TAG = "ui";

#define PAUSE( SEC ) for (int i = SEC; i > 0; i--) { \
    printf("Restarting in %d seconds...\n", i); \
    delayMs(1000); \
 }

lcd_device_t * lcd_dev;

#ifdef __cplusplus
extern "C"
{
#endif

enum ret_codes_t ui_init_state( void );
enum ret_codes_t ui_error_state( void );
enum ret_codes_t ui_display_state( void );

enum ui_states_t ui_lookup_transitions( enum ui_states_t state, enum ret_codes_t code );

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_FSM_H

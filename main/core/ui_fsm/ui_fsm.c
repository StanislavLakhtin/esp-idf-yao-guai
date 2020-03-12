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

#define TRANSITION_COUNT 5
static const ui_transition_t ui_states[] = {
    { idle,     FSM_OK,   settings},
    { idle,     FSM_FAIL, error },
    { settings, FSM_OK,   settings },
    { settings, FSM_FAIL, error },
    { error,    FSM_OK,   idle }};


ui_states_t ui_lookup_transitions( ui_states_t state, enum ret_codes_t code ) {
  for ( int i = 0 ; i < TRANSITION_COUNT ; i++ ) {
    if ( ui_states[ i ].src == state && ui_states[ i ].ret_codes == code )
      return ui_states[ i ].dst;
  }
  return UI_UNKNOWN_STATE;
}

void lookup_input_events(uint32_t xTimeInMs) {
    btns_event_t event;
    if (xQueueReceive(kbrd_evnt_queue, &event, pdMS_TO_TICKS ( xTimeInMs ))) {
      if (current_input_handler != NULL )  {
        current_input_handler(event);
      } else ESP_LOGI( TAG, "NULL pointer input handler");
    }
}

enum ret_codes_t ui_error_view( void ) {
  // show error for a short time (?) and switch to init
  ESP_LOGE(TAG, "ERROR in UI");
  return FSM_OK;
}


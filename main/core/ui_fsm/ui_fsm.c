//
// Created by Stanislav Lakhtin on 04.03.2020.
//

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

enum ret_codes_t ui_error_state( void ) {
  // show error for a short time (?) and switch to init
  ESP_LOGE(TAG, "ERROR in UI");
  return FSM_OK;
}


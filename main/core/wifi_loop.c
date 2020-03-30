//
// Created by Stanislav Lakhtin on 06/02/2020.
//

#include "yao-guai.h"
#include "core/wifi_fsm/wifi_fsm.h"

// Основная логика:
// 1. Просканировать существующие сети
// 2. Найти известные -> Подключиться по приоритету (прочитать конфигурацию) или в [5]
// 3. Обновить дату по NTP (?)
// 4. По истечению threshold прервать соединение,
// 5. выждать несколько секунд и повторить всё вновь

static const state_fptr_t states_fn[] = { init_state,
                                          find_ap_state,
                                          connect_ap_state,
                                          process_messages_state,
                                          process_errors};

void main_connection_task(void * arg) {
  enum states_t cur_state = ENTRY_STATE;
  enum ret_codes_t rc;
  state_fptr_t fn;

  loop {
    fn = states_fn[cur_state];
    rc = fn();
    EventBits_t currentEvents = xEventGroupGetBits(xEvents);
    if ( currentEvents & WIFI_SCAN) {
      find_ap_state();  // update known APs
      xEventGroupClearBits(xEvents, WIFI_SCAN);
    }
    xEventGroupSetBits(xEvents, WIFI_CHANGE_STATE);
    cur_state = lookup_transitions(cur_state, rc);
    vTaskDelay(2000 / portTICK_PERIOD_MS);  // make
  };
}
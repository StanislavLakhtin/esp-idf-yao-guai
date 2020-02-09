//
// Created by Stanislav Lakhtin on 06/02/2020.
//

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "yao-guai.h"
#include "tasks/wifi_fsm/wifi_fsm.h"

// Основная логика:
// 1. Просканировать существующие сети
// 2. Найти известные -> Подключиться по приоритету (прочитать конфигурацию) или в [5]
// 3. Обновить дату по NTP (?)
// 4. По истечению treshold прервать соединение,
// 5. выждать несколько секунд и повторить всё вновь

/*
 * struct transition state_transitions[] = {
    {init, ok,     find_ap},
    {init, fail,   error},
    {find_ap, ok,  connect_ap},
    {connect_ap,   fail,   error},
    {connect_ap,   ok, process_messages},
    {error,  ok, init},
    {process_messages,   fail,   error},
    {process_messages,   repeat, process_messages},
    {process_messages, ok, init}};
 */

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
    cur_state = lookup_transitions(cur_state, rc);
    ESP_LOGI( TAG, "Pause for 30 sec" );
    vTaskDelay(30000 / portTICK_PERIOD_MS);  // make
  };
}
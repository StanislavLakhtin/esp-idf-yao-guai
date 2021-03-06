/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI WIFI FSM

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

#include "wifi_fsm.h"

#define TRANSITION_COUNT 9    // всего 9 правил. Следует синхронизировать число с нижеследующей инициализацией самих правил
static const transition_t state_transitions[] = {
    {init,             FSM_OK,     find_ap},
    {init,             FSM_FAIL,   error},
    {find_ap,          FSM_OK,     connect_ap},
    {connect_ap,       FSM_FAIL,   error},
    {connect_ap,       FSM_OK,     process_messages},
    {error,            FSM_OK,     init},
    {process_messages, FSM_FAIL,   error},
    {process_messages, FSM_REPEAT, process_messages},
    {process_messages, FSM_OK,     init}};


enum states_t lookup_transitions(enum states_t state, enum ret_codes_t code) {
  for (int i = 0; i < TRANSITION_COUNT; i++) {
    if (state_transitions[i].src == state && state_transitions[i].ret_codes == code)
      return state_transitions[i].dst;
  }
  return UNKNOWN_STATE;
}

enum ret_codes_t init_state( void ) {
  return (wifi_init() == ESP_OK) ? FSM_OK : FSM_FAIL;
}

enum ret_codes_t find_ap_state( void ) {
  ESP_LOGI( TAG, "Clean previous WiFi scan");
  memset(ap_info, 0x00, sizeof(wifi_ap_record_t) * DEFAULT_AP_LIST_SIZE);
  ESP_ERROR_CHECK(wifi_scan(ap_info, &ap_cnt));
  ESP_LOGI( TAG, "Total APs scanned = %u", ap_cnt);
  for (int i = 0; i < ap_cnt; i++) {
    wifi_ap_record_t * ap = &ap_info[i];
    char _pr[32] = "";
    get_supported_proto_str(_pr, ap);
    ESP_LOGI(TAG, "[%d] %s, (%d dB) %s", i, ap->ssid, ap->rssi, _pr);
  }
  return FSM_OK;
}

enum ret_codes_t connect_ap_state( void ) {
  ESP_LOGI( TAG, "Connection to the desired Wifi Station..." );

  return FSM_OK;
}

enum ret_codes_t process_messages_state( void ) {
  //ESP_LOGI( TAG, "Process messages to MQTT broker" );
  return FSM_REPEAT;
}

enum ret_codes_t process_errors( void ) {
  ESP_LOGI( TAG, "Errors handling" );
  return FSM_OK;
}

void get_supported_proto_str(char* buffer, wifi_ap_record_t * ap) {
  if (ap->phy_11b || ap->phy_11g || ap->phy_11n)
    strcat(buffer, "802.11( ");
  IS_SUPPORTED(buffer, ap->phy_11b, "b");
  IS_SUPPORTED(buffer, ap->phy_11g, "g");
  IS_SUPPORTED(buffer, ap->phy_11n, "n");
  if (ap->phy_11b || ap->phy_11g || ap->phy_11n)
    strcat(buffer, " )");
  IS_SUPPORTED(buffer, ap->phy_lr, " LR");
  IS_SUPPORTED(buffer, ap->wps, " wps");
}
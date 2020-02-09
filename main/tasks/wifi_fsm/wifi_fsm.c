//
// Created by Stanislav Lakhtin on 09.02.2020.
//

#include "wifi_fsm.h"

static EventGroupHandle_t s_connect_event_group;
static esp_ip4_addr_t s_ip_addr;
static const char * s_connection_name;

#define GOT_IPV4_BIT BIT(0)
#define CONNECTED_BITS (GOT_IPV4_BIT)

// ---------------- Event Listeners ---------------------

static void on_got_ip( void * arg, esp_event_base_t event_base,
                       int32_t event_id, void * event_data ) {
  ESP_LOGI( TAG, "Got IP" );
  ip_event_got_ip_t * event = ( ip_event_got_ip_t * ) event_data;
  memcpy( &s_ip_addr, &event->ip_info.ip, sizeof( s_ip_addr ));
  xEventGroupSetBits( s_connect_event_group, GOT_IPV4_BIT);
}

static void on_wifi_disconnect( void * arg, esp_event_base_t event_base,
                                int32_t event_id, void * event_data ) {
  ESP_LOGI( TAG, "Wi-Fi disconnected" );
/*  esp_err_t err = esp_wifi_connect();
  if ( err == ESP_ERR_WIFI_NOT_STARTED) {
    return;
  }
  ESP_ERROR_CHECK( err );*/
}

static void on_wifi_scan_finished( void * arg, esp_event_base_t event_base,
                                   int32_t event_id, void * event_data ) {
  ESP_LOGI( TAG, "Wi-Fi scan complete" );
}

#define TRANSITION_COUNT 9    // всего 9 правил. Следует синхронизировать число с нижеследующей инициализацие правил
static const struct transition state_transitions[] = {
    {init, WIFI_OK,     find_ap},
    {init, WIFI_FAIL,   error},
    {find_ap, WIFI_OK,  connect_ap},
    {connect_ap,   WIFI_FAIL,   error},
    {connect_ap,   WIFI_OK, process_messages},
    {error,  WIFI_OK, init},
    {process_messages,   WIFI_FAIL,   error},
    {process_messages,   WIFI_REPEAT, process_messages},
    {process_messages, WIFI_OK, init}};


enum states_t lookup_transitions(enum states_t state, enum ret_codes_t code) {
  for (int i = 0; i < TRANSITION_COUNT; i++) {
    if (state_transitions[i].src == state && state_transitions[i].ret_codes == code)
      return state_transitions[i].dst;
  }
  return UNKNOWN_STATE;
}

enum ret_codes_t init_state(void ) {
  ESP_LOGI( TAG, "Initializing WiFi" );
  ap_cnt = 0x00;
  ap_info = NULL;
  netif = esp_netif_create_default_wifi_sta();
  assert( netif );

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init( &cfg ));

  ESP_ERROR_CHECK( esp_event_handler_register( WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_disconnect, NULL ));
  ESP_ERROR_CHECK( esp_event_handler_register( IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip, NULL ));
  ESP_ERROR_CHECK( esp_event_handler_register( WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &on_wifi_scan_finished, NULL ));
  ESP_ERROR_CHECK( wifi_start_station());
  return WIFI_OK;
}

enum ret_codes_t find_ap_state(void ) {
  if (ap_info != NULL) {
    ESP_LOGI( TAG, "Clean previous WiFi scan");
    free(ap_info);
    ap_info = NULL;
    ap_cnt = 0x00;
  }
  ap_info = malloc(sizeof(wifi_ap_record_t) * DEFAULT_AP_LIST_SIZE);
  ESP_ERROR_CHECK(wifi_scan(ap_info, &ap_cnt));
  ESP_LOGI( TAG, "Total APs scanned = %u", ap_cnt);
  for (int i = 0; i < ap_cnt; i++) {
    wifi_ap_record_t * ap = &ap_info[i];
    char _pr[32] = "";
    get_supported_proto_str(_pr, ap);
    ESP_LOGI(TAG, "[%d] %s, (%d dB) %s", i, ap->ssid, ap->rssi, _pr);
  }
  return WIFI_OK;
}

enum ret_codes_t connect_ap_state(void ) {
  ESP_LOGI( TAG, "Connect to desired Wifi Station" );
  return WIFI_OK;
}

enum ret_codes_t process_messages_state(void ) {
  ESP_LOGI( TAG, "Process messages to MQTT broker" );
  return WIFI_REPEAT;
}

enum ret_codes_t process_errors(void ) {
  ESP_LOGI( TAG, "Errors handling" );
  return WIFI_OK;
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
//
// Created by Stanislav Lakhtin on 06/02/2020.
//

#include "wifi_fsm.h"

// ---------------- Event Listeners ---------------------

void on_got_ip( void * arg, esp_event_base_t event_base,
                       int32_t event_id, void * event_data ) {
  ESP_LOGI( TAG, "Got IP event" );
  ip_event_got_ip_t * event = ( ip_event_got_ip_t * ) event_data;
  memcpy( &s_ip_addr, &event->ip_info.ip, sizeof( s_ip_addr ));
  ESP_LOGI( TAG, "IP: %d.%d.%d.%d / %d.%d.%d.%d", event->ip_info.ip.addr>>24,  // todo make ipv6 later
            event->ip_info.ip.addr>>16 & 0x000000ff,
            event->ip_info.ip.addr>>8 & 0x000000ff,
            event->ip_info.ip.addr & 0x000000ff,
            event->ip_info.netmask.addr>>24,
            event->ip_info.netmask.addr>>16 & 0x000000ff,
            event->ip_info.netmask.addr>>8 & 0x000000ff,
            event->ip_info.netmask.addr & 0x000000ff);
  xEventGroupSetBits( s_connect_event_group, CONNECTED_BITS);
}

void on_wifi_disconnect( void * arg, esp_event_base_t event_base,
                                int32_t event_id, void * event_data ) {
  ESP_LOGI( TAG, "Wi-Fi disconnected" );
}

void on_wifi_scan_finished( void * arg, esp_event_base_t event_base,
                                   int32_t event_id, void * event_data ) {
  ESP_LOGI( TAG, "Wi-Fi scan complete" );
}

esp_err_t wifi_start_station() {
  ESP_LOGI(TAG, "Setting station mode...");
  ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ));
  ESP_ERROR_CHECK( esp_wifi_start());
  if (current_connection_name == NULL) {
    current_connection_name = malloc(32);
    memset(current_connection_name, 0x00, 32);
  }
  return ESP_OK;
}

esp_err_t wifi_init() {
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
  return ESP_OK;
}

esp_err_t wifi_connect_to_ap(char* ssid, char* passwd) {
  if (s_connect_event_group != NULL) {
    return ESP_ERR_INVALID_STATE;
  }
  s_connect_event_group = xEventGroupCreate();
  start(ssid, passwd);
  ESP_ERROR_CHECK(esp_register_shutdown_handler(stop));
  ESP_LOGI(TAG, "Waiting for IP");
  xEventGroupWaitBits(s_connect_event_group, CONNECTED_BITS, true, true, portMAX_DELAY);
  ESP_LOGI(TAG, "Connected to %s", current_connection_name);
  ESP_LOGI(TAG, "IPv4 address: " IPSTR, IP2STR(&s_ip_addr));
#ifdef CONFIG_EXAMPLE_CONNECT_IPV6
  ESP_LOGI(TAG, "IPv6 address: " IPV6STR, IPV62STR(s_ipv6_addr));
#endif
  return ESP_OK;
}

void start(char* ssid, char* passwd) {

  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  wifi_config_t wifi_config;
  strncpy((char*) wifi_config.sta.ssid, ssid, 32);
  strncpy((char*) wifi_config.sta.password, passwd, 32);
  ESP_LOGI(TAG, "Connecting to %s...", wifi_config.sta.ssid);
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
  esp_err_t ret = esp_wifi_connect();
  switch (ret){
    default:
    case ESP_OK: break;
    case ESP_ERR_WIFI_NOT_INIT: break;
    case ESP_ERR_WIFI_NOT_STARTED: break;
    case ESP_ERR_WIFI_CONN: break;
    case ESP_ERR_WIFI_SSID: break;
  }
  strncpy(current_connection_name, (char*) wifi_config.sta.ssid, 32);
}

static void stop( void ) {
  ESP_ERROR_CHECK( esp_wifi_set_default_wifi_ap_handlers());

  esp_err_t err = esp_wifi_stop();
  if ( err == ESP_ERR_WIFI_NOT_INIT) {
    return;
  }
  ESP_ERROR_CHECK( err );
  ESP_ERROR_CHECK( esp_wifi_deinit());
  ESP_ERROR_CHECK( esp_wifi_clear_default_wifi_driver_and_handlers( netif ));
  esp_netif_destroy( netif );
}

esp_err_t wifi_scan( wifi_ap_record_t * buffer, uint16_t * cnt ) {
  uint16_t max_ap = DEFAULT_AP_LIST_SIZE;
  ESP_LOGI( TAG, "Scanning WiFi..." );
  esp_err_t _r = esp_wifi_scan_start( NULL, true );
  switch ( _r ) {
    default:
    case ESP_OK: break;
    case ESP_ERR_WIFI_NOT_INIT: ESP_LOGE(TAG, "Esp error: Wifi is not init"); return _r;
    case ESP_ERR_WIFI_NOT_STARTED: ESP_LOGE(TAG, "Esp error: Wifi is not started"); return _r;
    case ESP_ERR_WIFI_TIMEOUT: ESP_LOGE(TAG, "Esp error: Wifi timeout"); return _r;
    case ESP_ERR_WIFI_STATE: ESP_LOGE(TAG, "Unknown wifi error"); return _r;
  }
  ESP_ERROR_CHECK( esp_wifi_scan_get_ap_records( &max_ap, buffer ));
  ESP_ERROR_CHECK( esp_wifi_scan_get_ap_num( cnt ));
  return ESP_OK;
}
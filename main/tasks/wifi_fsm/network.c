//
// Created by Stanislav Lakhtin on 06/02/2020.
//

#include "wifi_fsm.h"

esp_err_t wifi_start_station() {
  ESP_LOGI(TAG, "Setting station mode...");
  ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ));
  ESP_ERROR_CHECK( esp_wifi_start());
  return ESP_OK;
}

/*static esp_err_t stop( conf_t * conf ) {
  ESP_ERROR_CHECK( esp_wifi_set_default_wifi_ap_handlers());

  esp_err_t err = esp_wifi_stop();
  if ( err == ESP_ERR_WIFI_NOT_INIT) {
    return ESP_ERR_ESP_NETIF_INVALID_PARAMS;
  }
  ESP_ERROR_CHECK( err );
  ESP_ERROR_CHECK( esp_wifi_deinit());
  ESP_ERROR_CHECK( esp_wifi_clear_default_wifi_driver_and_handlers( conf->wifi->netif ));
  esp_netif_destroy( conf->wifi->netif );
  return ESP_OK;
}


esp_err_t wifi_connect( conf_t* conf ) {
  if ( s_connect_event_group != NULL) {
    return ESP_ERR_INVALID_STATE;
  }
  s_connect_event_group = xEventGroupCreate();
  start(conf);
  ESP_ERROR_CHECK( esp_register_shutdown_handler( &stop ));
  ESP_LOGI( TAG, "Waiting for IP" );
  xEventGroupWaitBits( s_connect_event_group, CONNECTED_BITS, true, true, portMAX_DELAY );
  ESP_LOGI( TAG, "Connected to %s", s_connection_name );
  ESP_LOGI( TAG, "IPv4 address: "
  IPSTR, IP2STR( &s_ip_addr ));
  return ESP_OK;
}*/

esp_err_t wifi_scan( wifi_ap_record_t * buffer, uint16_t * cnt ) {
  uint16_t max_ap = DEFAULT_AP_LIST_SIZE;
  ESP_LOGI( TAG, "Scanning WiFi" );
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
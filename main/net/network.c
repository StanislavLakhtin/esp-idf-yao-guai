//
// Created by Stanislav Lakhtin on 06/02/2020.
//

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include <string.h>
#include "yao-guai.h"

#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_netif.h"
#include "driver/gpio.h"

#include "lwip/err.h"
#include "lwip/sys.h"

static EventGroupHandle_t s_connect_event_group;
static esp_ip4_addr_t s_ip_addr;
static const char * s_connection_name;

#define GOT_IPV4_BIT BIT(0)
#define CONNECTED_BITS (GOT_IPV4_BIT)
#define TAG "wifi"

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
  ESP_LOGI( TAG, "Wi-Fi disconnected, trying to reconnect..." );
  esp_err_t err = esp_wifi_connect();
  if ( err == ESP_ERR_WIFI_NOT_STARTED) {
    return;
  }
  ESP_ERROR_CHECK( err );
}

static void on_wifi_scan_finished( void * arg, esp_event_base_t event_base,
                                   int32_t event_id, void * event_data) {
  ESP_LOGI( TAG, "Wi-Fi scan complete" );
}

static void _pre_wifi_config(wifi_config_t* config, const wifi_conf_t* wifi_conf) {
  memset(config->sta.ssid, 0x00, 32);
  memset(config->sta.password, 0x00, 64);
  for (int i = 0; i < strlen(wifi_conf->ssid); i++) {
    config->sta.ssid[i] = wifi_conf->ssid[i];
  }
  for (int i = 0; i < strlen(wifi_conf->ssid_password); i++) {
    config->sta.password[i] = wifi_conf->ssid_password[i];
  }
}

esp_err_t wifi_init(conf_t* conf) {
  ESP_LOGI(TAG, "Initializing WiFi");
  conf->wifi->netif = esp_netif_create_default_wifi_sta();
  assert( conf->wifi->netif );

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init( &cfg ));

  ESP_ERROR_CHECK( esp_event_handler_register( WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_disconnect, NULL ));
  ESP_ERROR_CHECK( esp_event_handler_register( IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip, NULL ));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &on_wifi_scan_finished, NULL));
  return ESP_OK;
}

/*static void start(wifi_conf_t* wifi_conf) {
  wifi_config_t wifi_config;
  _pre_wifi_config(&wifi_config, wifi_conf);
  ESP_LOGI( TAG, "Connecting to %s...", wifi_config.sta.ssid );
  ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ));
  ESP_ERROR_CHECK( esp_wifi_set_config( ESP_IF_WIFI_STA, &wifi_config ));
  ESP_ERROR_CHECK( esp_wifi_start());
  ESP_ERROR_CHECK( esp_wifi_connect());
  s_connection_name = ssid;
}

static void stop( void ) {
  ESP_ERROR_CHECK( esp_event_handler_unregister( WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_disconnect ));
  ESP_ERROR_CHECK( esp_event_handler_unregister( IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip ));

  esp_err_t err = esp_wifi_stop();
  if ( err == ESP_ERR_WIFI_NOT_INIT) {
    return;
  }
  ESP_ERROR_CHECK( err );
  ESP_ERROR_CHECK( esp_wifi_deinit());
  ESP_ERROR_CHECK( esp_wifi_clear_default_wifi_driver_and_handlers( s_example_esp_netif ));
  esp_netif_destroy( s_example_esp_netif );
  s_example_esp_netif = NULL;
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

esp_err_t wifi_scan( conf_t* conf, bool fast_scan ) {
  ESP_LOGI(TAG, "Scanning WiFi");
  uint16_t ap_count = 0;
  memset(conf->wifi->ap_info, 0x00, sizeof(wifi_ap_record_t) * DEFAULT_AP_LIST_SIZE);
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&conf->wifi->ap_cnt, conf->wifi->ap_info));
  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
  ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
  return ESP_OK;
}
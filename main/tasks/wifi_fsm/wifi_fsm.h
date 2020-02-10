#pragma once
//
// Created by Stanislav Lakhtin on 09.02.2020.
//

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_WIFI_FSM_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_WIFI_FSM_H

#include "yao-guai.h"
#include <string.h>

#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_netif.h"
#include "driver/gpio.h"

typedef enum ret_codes_t ( * state_fptr_t ) ( void );

enum states_t {
      init = 0,
      find_ap = 1,
      connect_ap = 2,
      process_messages = 3,
      error = 4,
};

enum ret_codes_t { WIFI_OK, WIFI_FAIL, WIFI_REPEAT};

struct transition {
  enum states_t src;
  enum ret_codes_t ret_codes;
  enum states_t dst;
};

#define ENTRY_STATE init
#define UNKNOWN_STATE init

EventGroupHandle_t s_connect_event_group;
esp_ip4_addr_t s_ip_addr;
char * current_connection_name;

#define IS_SUPPORTED(buffer, flag, descr)  \
  do { \
    if (flag) \
      strcat(buffer, descr); \
  } while (0)

static const char* TAG = "wifi";
uint16_t ap_cnt;
wifi_ap_record_t * ap_info;
esp_netif_t * netif;

#define GOT_IPV4_BIT BIT(0)   // IPv4
#define GOT_IPV6_BIT BIT(1)   // IPv6

#ifdef CONFIG_CONNECT_IPV6
#define CONNECTED_BITS (GOT_IPV4_BIT | GOT_IPV6_BIT)
#else
#define CONNECTED_BITS (GOT_IPV4_BIT)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

enum ret_codes_t init_state(void );
enum ret_codes_t find_ap_state(void );
enum ret_codes_t connect_ap_state(void );
enum ret_codes_t process_messages_state(void );
enum ret_codes_t process_errors(void );

enum states_t lookup_transitions(enum states_t state, enum ret_codes_t code);
void get_supported_proto_str(char* buffer, wifi_ap_record_t * ap);

// ---------- wifi sta fns -----------------
esp_err_t wifi_init();
esp_err_t wifi_start_station();
esp_err_t wifi_scan( wifi_ap_record_t * buffer, uint16_t * cnt );
esp_err_t wifi_connect_to_ap(char* ssid, char* passwd);
void start(char* ssid, char* passwd);
static esp_err_t stop( void );
// ---------- wifi event listeners ----------
void on_got_ip( void * arg, esp_event_base_t event_base,
                       int32_t event_id, void * event_data );
void on_wifi_disconnect( void * arg, esp_event_base_t event_base,
                                int32_t event_id, void * event_data );
void on_wifi_scan_finished( void * arg, esp_event_base_t event_base,
                            int32_t event_id, void * event_data );

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_WIFI_FSM_H

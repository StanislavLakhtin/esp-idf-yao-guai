//
// Created by Stanislav Lakhtin on 06/02/2020.
//

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "yao-guai.h"

#define IS_SUPPORTED(buffer, flag, descr)  \
  do { \
    if (flag) \
      strcat(buffer, descr); \
  } while (0)

// Основная логика:
// 1. Просканировать существующие сети
// 2. Найти известные -> Подключиться по приоритету (прочитать конфигурацию) или в [5]
// 3. Обновить дату по NTP (?)
// 4. По истечению treshold прервать соединение,
// 5. выждать несколько секунд и повторить всё вновь

static const char* TAG = "wifi_loop";
char ssid[33];
char ssid_password[65];
uint16_t ap_cnt = 0x00;
wifi_ap_record_t * ap_info = NULL;
esp_netif_t* netif = NULL;

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

void main_connection_task(void * arg) {
  netif = wifi_init();
  ESP_ERROR_CHECK( wifi_start_station());
  loop {
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
    vTaskDelay(30000 / portTICK_PERIOD_MS);
  }
}
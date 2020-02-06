//
// Created by Stanislav Lakhtin on 06/02/2020.
//

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "yao-guai.h"

// Основная логика:
// 1. Просканировать существующие сети
// 2. Найти известные -> Подключиться по приоритету (прочитать конфигурацию) или в [5]
// 3. Обновить дату по NTP (?)
// 4. По истечению treshold прервать соединение,
// 5. выждать несколько секунд и повторить всё вновь

static const char* TAG = "wifi_loop";

void main_connection_task(void * arg) {
  conf_t* conf = arg;
  loop {
    ESP_ERROR_CHECK( wifi_start_station( conf ));
    ESP_ERROR_CHECK(wifi_scan(conf, false));
    ESP_LOGI( TAG, "Total APs scanned = %u", conf->wifi->ap_cnt );
    /*for (int i = 0; i < conf->wifi->ap_cnt; i++) {
      wifi_ap_record_t * ap = &conf->wifi->ap_info[i];
      ESP_LOGI(TAG, "[%d] %s, (%d dB)", i, ap->ssid, ap->rssi);
    }*/
    vTaskDelay(30000 / portTICK_PERIOD_MS);
  }
}
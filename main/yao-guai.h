#pragma once
//
// Created by Stanislav Lakhtin on 05/02/2020.
//

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/sdmmc_types.h"

#include "nvs_flash.h"

#include "esp_log.h"

#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"
#include "esp_event.h"

#include "ow_uart_driver/ow_uart_driver.h"

#define DEFAULT_AP_LIST_SIZE 10

typedef struct {
  char ssid[32];
  char ssid_password[64];
  esp_netif_t * netif;
  uint16_t ap_cnt;
  wifi_ap_record_t ap_info[DEFAULT_AP_LIST_SIZE];     // определяет, сколько всего может быть просканировано станций
} wifi_conf_t;

typedef struct {
  wifi_conf_t *wifi;
  SemaphoreHandle_t inUse;
  sdmmc_card_t *sd_card;
} conf_t;

#define PIN_NUM_MISO 4
#define PIN_NUM_MOSI 15
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   13

#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipse_org_pem_start[]  = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipse_org_pem_start[]   asm("_binary_mqtt_eclipse_org_pem_start");
#endif
extern const uint8_t mqtt_eclipse_org_pem_end[]   asm("_binary_mqtt_eclipse_org_pem_end");

#ifdef __cplusplus
extern "C"
{
#endif

// --------------- Tasks ---------------
void ow_periodically_scan_task(void *arg);

void main_connection_task(void *arg);

// ---------------- WiFi ----------------
esp_err_t wifi_init(conf_t *conf_t);

esp_err_t wifi_scan(conf_t *conf, bool fast_scan);

esp_err_t wifi_connect(conf_t *conf);

// SD-Card
esp_err_t conf_init(conf_t *conf);

esp_err_t sdcard_session_start(conf_t *conf);

esp_err_t sdcard_session_finish(conf_t *conf);

esp_err_t get_ap_password(conf_t *conf, char *ap_name, char *password_buffer);

esp_err_t save_scanned_ap(conf_t *conf);

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_H

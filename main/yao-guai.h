#pragma once
//
// Created by Stanislav Lakhtin on 05/02/2020.
//

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "ow_uart_driver/ow_uart_driver.h"
#include "lvgl.h"

#include "ui_loop.h"

#define DEFAULT_AP_LIST_SIZE 22

uint16_t ap_cnt;
wifi_ap_record_t ap_info[DEFAULT_AP_LIST_SIZE];

#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipse_org_pem_start[]  = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipse_org_pem_start[]   asm("_binary_mqtt_eclipse_org_pem_start");
#endif
extern const uint8_t mqtt_eclipse_org_pem_end[]   asm("_binary_mqtt_eclipse_org_pem_end");

#define WIFI_CHANGE_STATE  BIT(0)
#define WIFI_SCAN          BIT(1)

EventGroupHandle_t xEvents;

#define ESP_SSID_MAX_LENGTH 33
#define ESP_SSID_PASSWORD_MAX_LENGTH 65

typedef struct {
  char ssid[ESP_SSID_MAX_LENGTH];
  char password[ESP_SSID_PASSWORD_MAX_LENGTH];
  uint8_t priority;
} stored_ssid_conf_t;

stored_ssid_conf_t * stored_aps;
uint8_t stored_aps_count;
char conf_version[33];

TaskHandle_t * ui_task_handler;

#ifdef __cplusplus
extern "C"
{
#endif

// --------------- Tasks ---------------
void ow_periodically_scan_task(void *arg);
void main_connection_task(void *arg);
void ui_task(void *arg);


// --------------- Tools ---------------
esp_err_t load_conf(stored_ssid_conf_t * conf, uint8_t * count);
esp_err_t store_ap_to_conf(stored_ssid_conf_t * new_ap_ssid);
esp_err_t store_conf();

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_H

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

#include "driver/gpio.h"

#include "lvgl.h"
#include "nvs_flash.h"

#include "esp_log.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"
#include "esp_event.h"

#include "ow_uart_driver/ow_uart_driver.h"

#define DEFAULT_AP_LIST_SIZE 22

uint16_t ap_cnt;
wifi_ap_record_t ap_info[DEFAULT_AP_LIST_SIZE];

xQueueHandle kbrd_evnt_queue;

#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipse_org_pem_start[]  = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipse_org_pem_start[]   asm("_binary_mqtt_eclipse_org_pem_start");
#endif
extern const uint8_t mqtt_eclipse_org_pem_end[]   asm("_binary_mqtt_eclipse_org_pem_end");

#define WIFI_CHANGE_STATE  BIT(0)
#define WIFI_SCAN          BIT(1)

EventGroupHandle_t xEvents;

typedef struct stat filestat;

#define LIST_STORED_SSIDs_SIZE 10
typedef struct {
  uint8_t ssid[32];
  uint8_t password[64];
  uint8_t priority;
} stored_ssid_conf_t;

TaskHandle_t * ui_task_handler;

#ifdef __cplusplus
extern "C"
{
#endif

// --------------- Tasks ---------------
void ow_periodically_scan_task(void *arg);
void main_connection_task(void *arg);
void ui_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_H

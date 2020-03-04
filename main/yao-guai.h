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
#include "driver/sdmmc_defs.h"
#include "driver/gpio.h"

#include "nvs_flash.h"

#include "esp_log.h"

#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"
#include "esp_event.h"

#include <sdmmc_cmd.h>

#include "ow_uart_driver/ow_uart_driver.h"

#define DEFAULT_AP_LIST_SIZE 22

typedef struct {
  SemaphoreHandle_t inUse;
  sdmmc_card_t *sd_card;
} conf_t;

#define PIN_NUM_MISO 4
#define PIN_NUM_MOSI 15
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   13


#define GPIO_INPUT_ENCODER_0    35
#define GPIO_INPUT_ENCODER_1    33
#define GPIO_INPUT_ENCODER_BTN  34
#define GPIO_INPUT_PIN_SEL      ((1ULL<<GPIO_INPUT_ENCODER_0) | (1ULL<<GPIO_INPUT_ENCODER_1) | (1ULL<<GPIO_INPUT_ENCODER_BTN))

xQueueHandle kbrd_evnt_queue;
volatile TickType_t l_update_ts[3];
#define ENCODER_0     0x00
#define ENCODER_1     0x01
#define ENCODER_BTN   0x02
#define ENCODER_RT_THRESHOLD 20


#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipse_org_pem_start[]  = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipse_org_pem_start[]   asm("_binary_mqtt_eclipse_org_pem_start");
#endif
extern const uint8_t mqtt_eclipse_org_pem_end[]   asm("_binary_mqtt_eclipse_org_pem_end");

typedef struct stat filestat;

#define LIST_STORED_SSIDs_SIZE 10
typedef struct {
  uint8_t ssid[32];
  uint8_t password[64];
  uint8_t priority;
} stored_ssid_conf_t;

#ifdef __cplusplus
extern "C"
{
#endif

// --------------- Init ----------------
void conf_init(conf_t * conf);

// --------------- Tasks ---------------
void ow_periodically_scan_task(void *arg);

void main_connection_task(void *arg);

void display_task(void *arg);

// SD-Card
esp_err_t sdcard_session_start(conf_t *conf);

esp_err_t sdcard_session_finish(conf_t *conf);

esp_err_t is_ssid_conf_exists(conf_t *conf, const char * ssid);
esp_err_t get_ap_password(conf_t *conf, char *ap_name, char *password_buffer);

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_H

//
// Created by Stanislav Lakhtin on 05.03.2020.
//

#include "yao-guai.h"
#include <string.h>

#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_netif.h"
#include "driver/gpio.h"

typedef enum ret_codes_t ( * state_fptr_t ) ( void );

enum ret_codes_t { FSM_OK, FSM_FAIL, FSM_REPEAT, UI_WIFI_SCAN_COMPLETE, UI_BACK };

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_FSM_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_FSM_H

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_FSM_H

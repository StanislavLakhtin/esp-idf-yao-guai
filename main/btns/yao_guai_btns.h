//
// Created by Stanislav Lakhtin on 05/03/2020.
//

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_BTNS_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_BTNS_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/sdmmc_defs.h"
#include "driver/gpio.h"


volatile TickType_t l_update_ts[3];
#define ENCODER_0     0x00
#define ENCODER_1     0x01
#define ENCODER_BTN   0x02
#define ENCODER_RT_THRESHOLD 12

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_BTNS_H

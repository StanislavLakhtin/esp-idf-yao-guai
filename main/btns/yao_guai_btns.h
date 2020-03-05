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

typedef enum {
  rotate_left_begin,
  rotate_right_begin,
  rotate_finish,
  dormancy
} encoder_state_t;

typedef struct {
  gpio_num_t l_pin;
  gpio_num_t r_pin;
  encoder_state_t state;
  TickType_t last_update;
} encoder_t;

typedef  enum {
  BTN_ON_PRESS,
//  BTN_ON_RELEASE,
//  BTN_ON_HOLD,
  BTN_IDLE
} button_state_t;

typedef enum {
  ENCODER0_ROTATE_LEFT = '<',
  ENCODER0_ROTATE_RIGHT = '>',
  ENCODER0_PRESS = 'e',
  ENCODER0_RELEASE = 'E',
} btns_event_t;

typedef struct {
  gpio_num_t pin;
  button_state_t state;
  btns_event_t on_press_event;
  btns_event_t on_release_event;
} button_t;



#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_YAO_GUAI_BTNS_H

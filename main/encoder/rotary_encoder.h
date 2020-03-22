//
// Created by sl on 22.03.2020.
//

#ifndef YAO_GUAI_ROTARY_ENCODER_H
#define YAO_GUAI_ROTARY_ENCODER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/sdmmc_defs.h"
#include "driver/gpio.h"

// BTNS pins

#define GPIO_INPUT_ENCODER_0    35
#define GPIO_INPUT_ENCODER_1    33
#define GPIO_INPUT_ENCODER_BTN  34
#define GPIO_INPUT_PIN_SEL      ((1ULL<<GPIO_INPUT_ENCODER_0) | (1ULL<<GPIO_INPUT_ENCODER_1) | (1ULL<<GPIO_INPUT_ENCODER_BTN))

#define ESP_INTR_FLAG_DEFAULT 0

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
  TickType_t last_update;
} button_t;

xQueueHandle event_queue;

#define BUTTON_PRESS_THRESHOLD 10

#ifdef __cplusplus
extern "C"
{
#endif

void encoders_conf(xQueueHandle evnt_queue);

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_ROTARY_ENCODER_H

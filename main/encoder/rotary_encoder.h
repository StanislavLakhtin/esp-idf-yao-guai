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
  BTN_ON_PRESS,
  BTN_IDLE
} button_state_t;

typedef enum {
  ENCODER_IDLE = 0,
  LEFT_ROTATION_BEGIN = 1,
  RIGHT_ROTATION_BEGIN = 2,
} encoder_state_t;

typedef enum {
  ENCODER0_ROTATE_LEFT = '<',
  ENCODER0_ROTATE_RIGHT = '>',
  ENCODER0_PRESS = 'e',
  ENCODER0_RELEASE = 'E',
} btns_event_t;

typedef struct {
  gpio_num_t l_pin;
  gpio_num_t r_pin;
  encoder_state_t state;
  btns_event_t on_left;
  btns_event_t on_right;
} encoder_t;

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

void encoders_conf();
void button_init(button_t * btn, gpio_num_t pin, btns_event_t on_press, btns_event_t on_release);
void encoder_init(encoder_t * encoder, gpio_num_t l_pin, gpio_num_t r_pin, btns_event_t on_left, btns_event_t on_right);

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_ROTARY_ENCODER_H

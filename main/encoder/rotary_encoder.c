//
// Created by sl on 22.03.2020.
//

#include "rotary_encoder.h"

static encoder_state_t idle_cb(int l_pin, int r_pin);
static encoder_state_t left_rot_beg_cb(int l_pin, int r_pin);
static encoder_state_t right_rot_beg_cb(int l_pin, int r_pin);
static encoder_state_t left_rot_zero_cb(int l_pin, int r_pin);
static encoder_state_t right_rot_zero_cb(int l_pin, int r_pin);
static encoder_state_t left_rot_pre_cb(int l_pin, int r_pin);
static encoder_state_t right_rot_pre_cb(int l_pin, int r_pin);
static encoder_state_t undefined_zero_cb(int l_pin, int r_pin);

#define ENCODER_TRANSITION_STATES 11
static btns_event_t ROTATE_LEFT = ENCODER0_ROTATE_LEFT;
static btns_event_t ROTATE_RIGHT = ENCODER0_ROTATE_RIGHT;
static const encoder_fsm_transition_t encoder_fsm_transition[ENCODER_TRANSITION_STATES] = {
    {ENCODER_IDLE,              LEFT_ROTATION_BEGIN,       NULL},
    {ENCODER_IDLE,              RIGHT_ROTATION_BEGIN,      NULL},
    {ENCODER_IDLE,              UNDEFINED_ZERO_STATE,      NULL},
    {LEFT_ROTATION_BEGIN,       LEFT_ROTATION_ZERO_STATE,  NULL},
    {LEFT_ROTATION_ZERO_STATE,  LEFT_ROTATION_PRE_END,     NULL},
    {LEFT_ROTATION_PRE_END,     ENCODER_IDLE, &ROTATE_LEFT},
    {RIGHT_ROTATION_BEGIN,      RIGHT_ROTATION_ZERO_STATE, NULL},
    {RIGHT_ROTATION_ZERO_STATE, RIGHT_ROTATION_PRE_END,    NULL},
    {RIGHT_ROTATION_PRE_END,    ENCODER_IDLE, &ROTATE_RIGHT},
    {UNDEFINED_ZERO_STATE,      LEFT_ROTATION_PRE_END,     NULL},
    {UNDEFINED_ZERO_STATE,      RIGHT_ROTATION_PRE_END,    NULL},
};

static const encoder_state_fsm_fptr_t fsm_cb[] = {
    idle_cb,              // 0
    left_rot_beg_cb,      // 1
    right_rot_beg_cb,     // 2
    left_rot_zero_cb,     // 3
    right_rot_zero_cb,    // 4
    left_rot_pre_cb,      // 5
    right_rot_pre_cb,     // 6
    undefined_zero_cb};   // 7

encoder_state_t idle_cb(int l_pin, int r_pin) {
  if (!l_pin && !r_pin)
    return UNDEFINED_ZERO_STATE;
  return (!l_pin && r_pin) ? LEFT_ROTATION_BEGIN : RIGHT_ROTATION_BEGIN;
}

encoder_state_t left_rot_beg_cb(int l_pin, int r_pin) {
  return (!l_pin && !r_pin) ? LEFT_ROTATION_ZERO_STATE : ENCODER_IDLE;
}

encoder_state_t right_rot_beg_cb(int l_pin, int r_pin) {
  return (!l_pin && !r_pin) ? RIGHT_ROTATION_ZERO_STATE : ENCODER_IDLE;
}

encoder_state_t left_rot_zero_cb(int l_pin, int r_pin) {
  return (l_pin && !r_pin) ? LEFT_ROTATION_PRE_END : ENCODER_IDLE;
}

encoder_state_t right_rot_zero_cb(int l_pin, int r_pin) {
  return (!l_pin && r_pin) ? RIGHT_ROTATION_PRE_END : ENCODER_IDLE;
}

encoder_state_t left_rot_pre_cb(int l_pin, int r_pin) {
  return ENCODER_IDLE;
}

encoder_state_t right_rot_pre_cb(int l_pin, int r_pin) {
  return ENCODER_IDLE;
}

encoder_state_t undefined_zero_cb(int l_pin, int r_pin) {
  return (!l_pin && r_pin) ? RIGHT_ROTATION_PRE_END : LEFT_ROTATION_PRE_END;
}

void IRAM_ATTR encoder_isr_handler(void *arg) {
  encoder_t *encoder = arg;
  //TickType_t tick = xTaskGetTickCountFromISR();
  encoder_state_fsm_fptr_t fn = fsm_cb[encoder->state];
  encoder_state_t new_state = fn(gpio_get_level(encoder->l_pin), gpio_get_level(encoder->r_pin));
  for (uint8_t i = 0; i < ENCODER_TRANSITION_STATES; i++) {
    encoder_fsm_transition_t tr = encoder_fsm_transition[i];
    if (tr.from == encoder->state && tr.to == new_state) {
      encoder->state = new_state;
      if (tr.event != NULL) {
        xQueueSendFromISR(event_queue, tr.event, NULL);
      }
      return;
    }
  }
  encoder->state = ENCODER_IDLE;
}

void IRAM_ATTR buttons_isr_handler(void *arg) {
  button_t *button = arg;
  int gpio = gpio_get_level(button->pin);
  TickType_t tick;
  switch (button->state) {
    case BTN_IDLE:
      tick = xTaskGetTickCountFromISR();
      if (!gpio && (tick - button->last_update > BUTTON_PRESS_THRESHOLD)) {
        btns_event_t event = button->on_press_event;
        xQueueSendFromISR(event_queue, &event, NULL);
        button->state = BTN_ON_PRESS;
        button->last_update = tick;
      }
      break;
    case BTN_ON_PRESS:
      if (gpio) {
        btns_event_t event = button->on_release_event;
        xQueueSendFromISR(event_queue, &event, NULL);
        button->state = BTN_IDLE;
      }
      break;
  }
}

void encoders_conf(xQueueHandle queue) {
  event_queue = queue;

  gpio_config_t gpio_conf;
  gpio_conf.intr_type = GPIO_INTR_ANYEDGE;
  gpio_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  gpio_conf.mode = GPIO_MODE_INPUT;
  gpio_conf.pull_up_en = 1;
  gpio_config(&gpio_conf);

  encoder_t encoder = {
      .l_pin = GPIO_INPUT_ENCODER_0,
      .r_pin = GPIO_INPUT_ENCODER_1,
      .state = ENCODER_IDLE
  };

  button_t encoder_btn = {
      .pin = GPIO_INPUT_ENCODER_BTN,
      .state = BTN_IDLE,
      .on_press_event = ENCODER0_PRESS,
      .on_release_event = ENCODER0_RELEASE
  };

  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  gpio_isr_handler_add(encoder.l_pin, encoder_isr_handler, &encoder);
  gpio_isr_handler_add(encoder.r_pin, encoder_isr_handler, &encoder);
  gpio_isr_handler_add(GPIO_INPUT_ENCODER_BTN, buttons_isr_handler, &encoder_btn);
}
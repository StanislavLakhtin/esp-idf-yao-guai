//
// Created by sl on 22.03.2020.
//

#include "rotary_encoder.h"

void IRAM_ATTR encoder_isr_handler(void *arg) {
  encoder_t *encoder = arg;
  int l_pin = gpio_get_level(encoder->l_pin);
  int r_pin = gpio_get_level(encoder->r_pin);
  TickType_t tick = xTaskGetTickCountFromISR();
  if (tick - encoder->last_update > 10) {
    encoder->last_update = tick;
    encoder->state = ENCODER_IDLE;
  }
  switch (encoder->state) {
    case ENCODER_IDLE:
      if (!l_pin && r_pin)
        encoder->state = LEFT_ROTATION_BEGIN;
      if (l_pin && !r_pin)
        encoder->state = RIGHT_ROTATION_BEGIN;
      encoder->last_update = tick;
      break;
    case LEFT_ROTATION_BEGIN:
      if (!r_pin) {
        btns_event_t event = encoder->on_left;
        xQueueSendFromISR(event_queue, &event, NULL);
        encoder->state = ENCODER_IDLE;
      }
      break;
    case RIGHT_ROTATION_BEGIN:
      if (!l_pin) {
        btns_event_t event = encoder->on_right;
        xQueueSendFromISR(event_queue, &event, NULL);
        encoder->state = ENCODER_IDLE;
      }
      break;
  }
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

button_t encoder_btn;
encoder_t encoder0;

void encoders_conf() {
  event_queue = xQueueCreate(5, sizeof(btns_event_t));

  gpio_config_t gpio_conf = {
      .intr_type = GPIO_INTR_NEGEDGE,
      .pin_bit_mask = GPIO_INPUT_PIN_SEL,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = 1
  };
  gpio_config(&gpio_conf);
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

  button_init(&encoder_btn, GPIO_INPUT_ENCODER_BTN, ENCODER0_PRESS, ENCODER0_RELEASE);
  encoder_init(&encoder0, GPIO_INPUT_ENCODER_0, GPIO_INPUT_ENCODER_1, ENCODER0_ROTATE_LEFT, ENCODER0_ROTATE_RIGHT);
}

void button_init(button_t * btn, gpio_num_t pin, btns_event_t on_press, btns_event_t on_release) {
  btn->pin = pin;
  btn->state = BTN_IDLE;
  btn->on_press_event = on_press;
  btn->on_release_event = on_release;
  gpio_isr_handler_add(pin, buttons_isr_handler, btn);
}

void encoder_init(encoder_t * encoder, gpio_num_t l_pin, gpio_num_t r_pin, btns_event_t on_left, btns_event_t on_right) {
  encoder->l_pin = l_pin;
  encoder->r_pin = r_pin;
  encoder->state = ENCODER_IDLE;
  encoder->on_left = on_left;
  encoder->on_right = on_right;
  gpio_isr_handler_add(encoder->l_pin, encoder_isr_handler, encoder);
  gpio_isr_handler_add(encoder->r_pin, encoder_isr_handler, encoder);
}
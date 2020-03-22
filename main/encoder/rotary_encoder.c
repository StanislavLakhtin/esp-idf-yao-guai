//
// Created by sl on 22.03.2020.
//

#include "rotary_encoder.h"

encoder_t encoder0;
button_t encoder_btn;

void IRAM_ATTR encoder_isr_handler(void* arg) {
  encoder_t * encoder = arg;
  //todo make full FSM machine
  //TickType_t tick = xTaskGetTickCountFromISR();
  int l_pin, r_pin;
  l_pin = gpio_get_level(encoder->l_pin);
  r_pin = gpio_get_level(encoder->r_pin);
  switch (encoder->state) {
    case dormancy:
      if (r_pin && !l_pin) {
        encoder->state = rotate_left_begin;
      } else if (!r_pin && l_pin) {
        encoder->state = rotate_right_begin;
      }
      break;
    case rotate_left_begin:
      if (!r_pin && !l_pin) {
        btns_event_t event = ENCODER0_ROTATE_LEFT;
        //xQueueOverwriteFromISR(kbrd_evnt_queue, &event, NULL);
        xQueueSendFromISR(event_queue, &event, NULL);
        encoder->state = rotate_finish;
      }
      break;
    case rotate_right_begin:
      if (!l_pin && !l_pin) {
        btns_event_t event = ENCODER0_ROTATE_RIGHT;
        xQueueSendFromISR(event_queue, &event, NULL);
        encoder->state = rotate_finish;
      }
      break;
    default:
    case rotate_finish:
      if (r_pin && l_pin) {
        encoder->state = dormancy;
      }
      break;
  }
}

void IRAM_ATTR buttons_isr_handler(void* arg) {
  button_t * button = arg;
  int gpio = gpio_get_level(button->pin);
  TickType_t tick;
  switch (button->state) {
    case BTN_IDLE:
      tick = xTaskGetTickCountFromISR();
      if (!gpio && (tick - button->last_update > BUTTON_PRESS_THRESHOLD) ) {
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
  event_queue = queue ;

  gpio_config_t gpio_conf;
  gpio_conf.intr_type = GPIO_INTR_NEGEDGE;
  gpio_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  gpio_conf.mode = GPIO_MODE_INPUT;
  gpio_conf.pull_up_en = 1;
  gpio_config(&gpio_conf);

  encoder0.l_pin = GPIO_INPUT_ENCODER_0;
  encoder0.r_pin = GPIO_INPUT_ENCODER_1;
  encoder0.state = dormancy;

  encoder_btn.pin = GPIO_INPUT_ENCODER_BTN;
  encoder_btn.state = BTN_IDLE;
  encoder_btn.on_press_event = ENCODER0_PRESS;
  encoder_btn.on_release_event = ENCODER0_RELEASE;

  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  gpio_isr_handler_add(encoder0.l_pin, encoder_isr_handler, &encoder0);
  gpio_isr_handler_add(encoder0.r_pin, encoder_isr_handler, &encoder0);
  gpio_isr_handler_add(GPIO_INPUT_ENCODER_BTN, buttons_isr_handler, &encoder_btn);
}
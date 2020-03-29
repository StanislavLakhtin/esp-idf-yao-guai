//
// Created by sl on 22.03.2020.
//

#include "rotary_encoder.h"

void IRAM_ATTR encoder_isr_handler(void *arg) {
  encoder_t *encoder = arg;
  btns_event_t event = (gpio_get_level(encoder->dir_pin)) ? encoder->on_right : encoder->on_left;
  xQueueSendFromISR(event_queue, &event, NULL);
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
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

  event_queue = xQueueCreate(5, sizeof(btns_event_t));

  button_init(&encoder_btn, GPIO_INPUT_ENCODER_BTN, ENCODER0_PRESS, ENCODER0_RELEASE);
  encoder_init(&encoder0, GPIO_INPUT_DIR_PIN, GPIO_INPUT_INT_PIN, ENCODER0_ROTATE_LEFT, ENCODER0_ROTATE_RIGHT);

}

void button_init(button_t * btn, gpio_num_t pin, btns_event_t on_press, btns_event_t on_release) {
  btn->pin = pin;
  btn->state = BTN_IDLE;
  btn->on_press_event = on_press;
  btn->on_release_event = on_release;

  gpio_set_direction(btn->pin, GPIO_MODE_INPUT);
  gpio_set_pull_mode(btn->pin, GPIO_PULLUP_ONLY);
  gpio_set_intr_type(btn->pin, GPIO_INTR_ANYEDGE);
  gpio_isr_handler_add(pin, buttons_isr_handler, btn);
}

void encoder_init(encoder_t * encoder, gpio_num_t dir_pin, gpio_num_t int_pin, btns_event_t on_left, btns_event_t on_right) {
  encoder->dir_pin = dir_pin;
  encoder->int_pin = int_pin;
  encoder->on_left = on_left;
  encoder->on_right = on_right;

  gpio_set_direction(encoder->dir_pin, GPIO_MODE_INPUT);
  gpio_set_direction(encoder->int_pin, GPIO_MODE_INPUT);
  gpio_set_pull_mode(encoder->dir_pin, GPIO_PULLUP_ONLY);
  gpio_set_pull_mode(encoder->int_pin, GPIO_PULLUP_ONLY);

  gpio_set_intr_type(int_pin, GPIO_INTR_POSEDGE);

  gpio_isr_handler_add(encoder->int_pin, encoder_isr_handler, encoder);
}
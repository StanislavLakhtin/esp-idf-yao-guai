/* Free RTOS Yao-Guai Weather meteostation
 *
 * Created by Stanislav Lakhtin on 02.02.2020.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "yao-guai.h"
#include "btns/yao_guai_btns.h"

void IRAM_ATTR gpio_isr_handler(void* arg) {
  encoder_t * encoder = arg;
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
      if (!r_pin) {
        btns_event_t event = ENCODER0_ROTATE_LEFT;
        xQueueSendFromISR(kbrd_evnt_queue, &event, NULL);
        encoder->state = rotate_finish;
      }
      break;
    case rotate_right_begin:
      if (!l_pin) {
        btns_event_t event = ENCODER0_ROTATE_RIGHT;
        xQueueSendFromISR(kbrd_evnt_queue, &event, NULL);
        encoder->state = rotate_finish;
      }
      break;
    case rotate_finish:
      if (r_pin && l_pin) {
        encoder->state = dormancy;
      }
      break;
  }
}

static void gpio_task(void* arg) {
  btns_event_t event;
  for(;;) {
    if(xQueueReceive(kbrd_evnt_queue, &event, portMAX_DELAY)) {
      printf("[ %c ]", event);
    }
  }
}

void app_main(void) {

  conf_t conf;
  conf_init(&conf);

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK( ret );
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  ESP_ERROR_CHECK(ow_uart_driver_init());

  xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);
  xTaskCreate(main_connection_task, "main_connection_task", 4096, NULL, 10, NULL);
  xTaskCreate(display_task, "display_task", 4096, NULL, 10, NULL);
  xTaskCreate(ow_periodically_scan_task, "ow_periodically_scan_task", 2048, NULL, 10, NULL);
}

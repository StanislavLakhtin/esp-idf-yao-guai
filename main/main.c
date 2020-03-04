/* Free RTOS Yao-Guai Weather meteostation
 *
 * Created by Stanislav Lakhtin on 02.02.2020.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "yao-guai.h"

#define ESP_INTR_FLAG_DEFAULT 0

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
  uint32_t gpio_num = (uint32_t) arg;
  xQueueSendFromISR(kbrd_evnt_queue, &gpio_num, NULL);
}

static void gpio_task(void* arg)
{
  uint32_t io_num;
  for(;;) {
    if(xQueueReceive(kbrd_evnt_queue, &io_num, portMAX_DELAY)) {
      printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
    }
  }
}

void app_main(void) {

  kbrd_evnt_queue = xQueueCreate(10, sizeof(uint32_t));

  gpio_config_t gpio_conf;
  gpio_conf.intr_type = GPIO_INTR_ANYEDGE;
  gpio_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  gpio_conf.mode = GPIO_MODE_INPUT;
  gpio_conf.pull_up_en = 1;
  gpio_config(&gpio_conf);

  conf_t conf;
  conf_init(&conf);

  //install gpio isr service
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  //hook isr handler for specific gpio pin
  gpio_isr_handler_add(GPIO_INPUT_ENCODER_0, gpio_isr_handler, (void*) GPIO_INPUT_ENCODER_0);
  //hook isr handler for specific gpio pin
  gpio_isr_handler_add(GPIO_INPUT_ENCODER_1, gpio_isr_handler, (void*) GPIO_INPUT_ENCODER_1);

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

  xTaskCreate(gpio_task, "gpio_task", 1024, NULL, 10, NULL);
  xTaskCreate(main_connection_task, "main_connection_task", 4096, NULL, 10, NULL);
  xTaskCreate(display_task, "display_task", 4096, NULL, 10, NULL);
  xTaskCreate(ow_periodically_scan_task, "ow_periodically_scan_task", 2048, NULL, 10, NULL);
}

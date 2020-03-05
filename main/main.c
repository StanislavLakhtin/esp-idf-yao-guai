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
  uint32_t btn_id = (uint32_t) arg;
  TickType_t tick = xTaskGetTickCountFromISR();
  if (btn_id < 2) {
    //encoder
    if (tick - l_update_ts[btn_id] > ENCODER_RT_THRESHOLD) {
      l_update_ts[btn_id] = tick;
      xQueueSendFromISR(kbrd_evnt_queue, &btn_id, NULL);
    }
  }
}

static void gpio_task(void* arg) {
  uint32_t io_num;
  for(;;) {
    if(xQueueReceive(kbrd_evnt_queue, &io_num, portMAX_DELAY)) {
      printf("[ %d ]", io_num);
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

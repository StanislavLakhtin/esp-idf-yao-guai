/* Free RTOS Yao-Guai Weather meteostation
 *
 * Created by Stanislav Lakhtin on 02.02.2020.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "yao-guai.h"

void app_main(void) {
  wifi_conf_t wifi_conf = {
      .ap_cnt = 0x00,
      .ap_info = NULL,
      .netif = NULL,
  };
  conf_t conf = {
      .wifi = &wifi_conf,
  };
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
  ESP_ERROR_CHECK(wifi_init(&conf));

  //xTaskCreate(ow_periodically_scan_task, "ow_periodically_scan_task", 2048, NULL, 10, NULL);
  xTaskCreate(main_connection_task, "main_connection_task", 4096, &conf, 10, NULL);
}

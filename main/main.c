/* Free RTOS Yao-Guai Weather meteostation
 *
 * Created by Stanislav Lakhtin on 02.02.2020.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "yao-guai.h"

void app_main(void) {
  conf_t conf;
  //ESP_ERROR_CHECK(ow_uart_driver_init());
  //ESP_ERROR_CHECK(conf_init(&conf));
  ESP_ERROR_CHECK(wifi_init(&conf));

  //xTaskCreate(ow_periodically_scan_task, "ow_periodically_scan_task", 2048, NULL, 10, NULL);
  xTaskCreate(main_connection_task, "main_connection_task", 4096, &conf, 10, NULL);
}

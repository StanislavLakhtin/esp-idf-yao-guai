/* Free RTOS Yao-Guai Weather meteostation
 *
 * Created by Stanislav Lakhtin on 02.02.2020.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "yao-guai.h"
#include "nvs_flash.h"

static const char * TAG = "LOADER";

#define WIFI_PROIRITY     5
#define ONEWIRE_PROIRITY  6
#define UI_PROIRITY       7

void app_main(void) {
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

  /* Print chip information */
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
         CONFIG_IDF_TARGET,
         chip_info.cores,
         (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  printf("silicon revision %d, ", chip_info.revision);

  printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
         (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  xEvents = xEventGroupCreate();
  if ( xEvents == NULL ) {
    ESP_LOGE(TAG, "The event group was not created because there was insufficient FreeRTOS heap available");
    esp_restart();
  }

  xTaskCreatePinnedToCore(main_connection_task, "main_connection_task", 4096, NULL, WIFI_PROIRITY, NULL, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(ui_task, "display_task", 4096, NULL, UI_PROIRITY, ui_task_handler, APP_CPU_NUM);
  xTaskCreatePinnedToCore(ow_periodically_scan_task, "ow_periodically_scan_task", 2048, NULL, ONEWIRE_PROIRITY, NULL, PRO_CPU_NUM);
}

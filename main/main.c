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

static const char * TAG = "LOADER";

void IRAM_ATTR encoder_isr_handler(void* arg) {
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

void IRAM_ATTR buttons_isr_handler(void* arg) {
  button_t * button = arg;
  int gpio = gpio_get_level(button->pin);
  TickType_t tick;
  switch (button->state) {
    case BTN_IDLE:
      tick = xTaskGetTickCountFromISR();
      if (!gpio && (tick - button->last_update > BUTTON_PRESS_THRESHOLD) ) {
        btns_event_t event = button->on_press_event;
        xQueueSendFromISR(kbrd_evnt_queue, &event, NULL);
        button->state = BTN_ON_PRESS;
        button->last_update = tick;
      }
      break;
    case BTN_ON_PRESS:
      if (gpio) {
        btns_event_t event = button->on_release_event;
        xQueueSendFromISR(kbrd_evnt_queue, &event, NULL);
        button->state = BTN_IDLE;
      }
      break;
  }
}

#define WIFI_PROIRITY     5
#define ONEWIRE_PROIRITY  6
#define UI_PROIRITY       7

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

  esp_chip_info_t chip_info;
  esp_chip_info( &chip_info );
  ESP_LOGI( TAG, "This is %s chip with %d CPU cores, WiFi%s%s, ",
            CHIP_NAME,
            chip_info.cores,
            ( chip_info.features & CHIP_FEATURE_BT ) ? "/BT" : "",
            ( chip_info.features & CHIP_FEATURE_BLE ) ? "/BLE" : "" );

  ESP_LOGI( TAG, "silicon revision %d, ", chip_info.revision );

  ESP_LOGI( TAG, "%dMB %s flash\n", spi_flash_get_chip_size() / ( 1024 * 1024 ),
            ( chip_info.features & CHIP_FEATURE_EMB_FLASH ) ? "embedded" : "external" );

  xEvents = xEventGroupCreate();
  if ( xEvents == NULL ) {
    ESP_LOGE(TAG, "The event group was not created because there was insufficient FreeRTOS heap available");
    esp_restart();
  }

  xTaskCreatePinnedToCore(main_connection_task, "main_connection_task", 4096, NULL, WIFI_PROIRITY, NULL, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(ui_task, "display_task", 4096, NULL, UI_PROIRITY, ui_task_handler, APP_CPU_NUM);
  xTaskCreatePinnedToCore(ow_periodically_scan_task, "ow_periodically_scan_task", 2048, NULL, ONEWIRE_PROIRITY, NULL, PRO_CPU_NUM);
}

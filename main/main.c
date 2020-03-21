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
#include "buttons_encoders.h"

static const char * TAG = "LOADER";

#define WIFI_PROIRITY     5
#define ONEWIRE_PROIRITY  6
#define UI_PROIRITY       7

// BTNS pins

#define GPIO_INPUT_ENCODER_0    35
#define GPIO_INPUT_ENCODER_1    33
#define GPIO_INPUT_ENCODER_BTN  34
#define GPIO_INPUT_PIN_SEL      ((1ULL<<GPIO_INPUT_ENCODER_0) | (1ULL<<GPIO_INPUT_ENCODER_1) | (1ULL<<GPIO_INPUT_ENCODER_BTN))

#define ESP_INTR_FLAG_DEFAULT 0

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
        xQueueSendFromISR(kbrd_evnt_queue, &event, NULL);
        encoder->state = rotate_finish;
      }
      break;
    case rotate_right_begin:
      if (!l_pin && !l_pin) {
        btns_event_t event = ENCODER0_ROTATE_RIGHT;
        xQueueSendFromISR(kbrd_evnt_queue, &event, NULL);
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

void encoders_conf(void);

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
  encoders_conf();
  if ( xEvents == NULL ) {
    ESP_LOGE(TAG, "The event group was not created because there was insufficient FreeRTOS heap available");
    esp_restart();
  }

  xTaskCreatePinnedToCore(main_connection_task, "main_connection_task", 4096, NULL, WIFI_PROIRITY, NULL, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(ui_task, "display_task", 4096, NULL, UI_PROIRITY, ui_task_handler, APP_CPU_NUM);
  xTaskCreatePinnedToCore(ow_periodically_scan_task, "ow_periodically_scan_task", 2048, NULL, ONEWIRE_PROIRITY, NULL, PRO_CPU_NUM);
}

void encoders_conf(void) {
  kbrd_evnt_queue = xQueueCreate(5, sizeof(uint32_t));

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

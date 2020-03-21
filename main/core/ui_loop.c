/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI LCD Driver Example

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.

    Support for several separate devices / screens in one library is antipattern.
    However, ST7789 and ILI-9340 have such a common interaction interface that combining them
    within a common library may make sense.

    This library is a compromise in this matter. The main goal is to ensure the speed of execution and
    the absence of checks on the time of execution.
*/
#include <buttons_encoders.h>
#include "yao-guai.h"

#include "esp_freertos_hooks.h"

#include "lvgl.h"
#include "lvgl_driver.h"

#include "screens.h"

/**********************
 *  STATIC VARIABLES
 **********************/
static const char * TAG = "ui";
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void IRAM_ATTR lv_tick_task(void) {
  lv_tick_inc(portTICK_RATE_MS);
}

static bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data) {
  btns_event_t event;
  if (xQueueReceive(kbrd_evnt_queue, &event, 0)) {
    ESP_LOGI(TAG, "%c", event);
    switch (event) {
      case ENCODER0_PRESS:
        data->enc_diff = LV_KEY_ENTER;
        break;
      case ENCODER0_ROTATE_LEFT:
        data->enc_diff = LV_KEY_LEFT;
        break;
      case ENCODER0_ROTATE_RIGHT:
        data->enc_diff = LV_KEY_RIGHT;
        break;
      default:
      case ENCODER0_RELEASE:
        break;
    }
    return true;
  }
  return false;
}

void ui_task(void * args ) {
  lv_init();

  lvgl_driver_init();

  static lv_color_t buf1[DISP_BUF_SIZE];
  static lv_color_t buf2[DISP_BUF_SIZE];
  static lv_disp_buf_t disp_buf;
  lv_disp_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = disp_driver_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  lv_indev_drv_init(&encoder_drv);
  encoder_drv.type = LV_INDEV_TYPE_ENCODER;
  encoder_drv.read_cb = read_encoder;
  encoder_indev = lv_indev_drv_register(&encoder_drv);

#if CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
  lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.read_cb = touch_driver_read;
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	lv_indev_drv_register(&indev_drv);
#endif

  esp_register_freertos_tick_hook(lv_tick_task);
  ESP_LOGI( TAG, "Hello world from Yao GUAI [meteo]station.\n" );

  construct_settings_screen();

  loop {
    vTaskDelay(1);
    lv_task_handler();
  };
}
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
#include "yao-guai.h"

#include "esp_freertos_hooks.h"

#include "lvgl.h"
#include "lvgl_driver.h"

static const char * TAG = "ui";

static void demo(void);

static void IRAM_ATTR lv_tick_task(void) {
  lv_tick_inc(portTICK_RATE_MS);
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

#if CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
  lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.read_cb = touch_driver_read;
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	lv_indev_drv_register(&indev_drv);
#endif

  esp_register_freertos_tick_hook(lv_tick_task);
  ESP_LOGI( TAG, "Hello world from Yao GUAI [meteo]station.\n" );

  demo();

  loop {
    vTaskDelay(1);
    lv_task_handler();
  };
}

void demo(void) {
  lv_obj_t * scr = lv_disp_get_scr_act(NULL);     /*Get the current screen*/

  /*Create a Label on the currently active screen*/
  lv_obj_t * label1 =  lv_label_create(scr, NULL);

  /*Modify the Label's text*/
  lv_label_set_text(label1, "Hello world!");

  /* Align the Label to the center
   * NULL means align on parent (which is the screen now)
   * 0, 0 at the end means an x, y offset after alignment*/
  lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
}
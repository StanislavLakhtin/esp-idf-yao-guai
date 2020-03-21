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

/**********************
 *  STATIC VARIABLES
 **********************/
static const char * TAG = "ui";
/**********************
 *  STATIC PROTOTYPES
 **********************/
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

static void demo(void) {
  lv_obj_t * scr = lv_disp_get_scr_act(NULL);     /*Get the current screen*/

  /****************************************
   * BASE OBJECT + LABEL WITH DEFAULT STYLE
   ****************************************/
  /*Create a simple objects*/
  lv_obj_t * obj1;
  obj1 = lv_obj_create(scr, NULL);
  lv_obj_set_pos(obj1, 10, 10);

  /*Add a label to the object*/
  lv_obj_t * label;
  label = lv_label_create(obj1, NULL);
  lv_label_set_text(label, "Default");
  lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

  /****************************************
   * BASE OBJECT WITH 'PRETTY COLOR' STYLE
   ****************************************/
  /*Create a simple objects*/
  lv_obj_t * obj2;
  obj2 = lv_obj_create(scr, NULL);
  lv_obj_align(obj2, obj1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);    /*Align next to the previous object*/
  lv_obj_set_style(obj2, &lv_style_pretty);                   /*Set built in style*/

  /* Add a label to the object.
   * Labels by default inherit the parent's style */
  label = lv_label_create(obj2, NULL);
  lv_label_set_text(label, "Pretty\nstyle");
  lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

  /*****************************
   * BASE OBJECT WITH NEW STYLE
   *****************************/
  /* Create a new style */
  static lv_style_t style_new;                         /*Styles can't be local variables*/
  lv_style_copy(&style_new, &lv_style_pretty);         /*Copy a built-in style as a starting point*/
  style_new.body.radius = 1;//LV_RADIUS_CIRCLE;            /*Fully round corners*/
  style_new.body.main_color = LV_COLOR_WHITE;          /*White main color*/
  style_new.body.grad_color = LV_COLOR_BLUE;           /*Blue gradient color*/
  style_new.body.shadow.color = LV_COLOR_SILVER;       /*Light gray shadow color*/
  style_new.body.shadow.width = 2;                     /*8 px shadow*/
  style_new.body.border.width = 1;                     /*2 px border width*/
  style_new.text.color = LV_COLOR_RED;                 /*Red text color */

  /*Create a base object and apply the new style*/
  lv_obj_t * obj3;
  obj3 = lv_obj_create(scr, NULL);
  lv_obj_align(obj3, obj2, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
  lv_obj_set_style(obj3, &style_new);

  /* Add a label to the object.
   * Labels by default inherit the parent's style */
  label = lv_label_create(obj3, NULL);
  lv_label_set_text(label, "New\nstyle");
  lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);


  /************************
   * CREATE A STYLED BAR
   ***********************/
  /* Create a bar background style */
  static lv_style_t style_bar_bg;
  lv_style_copy(&style_bar_bg, &lv_style_pretty);
  style_bar_bg.body.radius = 2;
  style_bar_bg.body.opa = LV_OPA_TRANSP;                  /*Empty (not filled)*/
  style_bar_bg.body.border.color = LV_COLOR_GRAY;         /*Gray border color*/
  style_bar_bg.body.border.width = 1;                     /*2 px border width*/
  style_bar_bg.body.border.opa = LV_OPA_COVER;

  /* Create a bar indicator style */
  static lv_style_t style_bar_indic;
  lv_style_copy(&style_bar_indic, &lv_style_pretty);
  style_bar_indic.body.radius = 2;
  style_bar_indic.body.main_color = LV_COLOR_GRAY;          /*White main color*/
  style_bar_indic.body.grad_color = LV_COLOR_GRAY;           /*Blue gradient color*/
  style_bar_indic.body.border.width = 0;                     /*2 px border width*/
  style_bar_indic.body.padding.left = 2;
  style_bar_indic.body.padding.right = 2;
  style_bar_indic.body.padding.top = 2;
  style_bar_indic.body.padding.bottom = 2;

  /*Create a bar and apply the styles*/
  lv_obj_t * bar = lv_bar_create(scr, NULL);
  lv_bar_set_style(bar, LV_BAR_STYLE_BG, &style_bar_bg);
  lv_bar_set_style(bar, LV_BAR_STYLE_INDIC, &style_bar_indic);
  lv_bar_set_value(bar, 70, false);
  lv_obj_set_size(bar, 200, 20);
  lv_obj_align(bar, obj1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

}


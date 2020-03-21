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
static lv_obj_t * slider;
static const char * TAG = "ui";
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void demo(void);
static void btn_event_cb(lv_obj_t * btn, lv_event_t event);
static void ddlist_event_cb(lv_obj_t * ddlist, lv_event_t event);

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

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Called when a button is released
 * @param btn pointer to the released button
 * @param event the triggering event
 * @return LV_RES_OK because the object is not deleted in this function
 */
static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
  if(event == LV_EVENT_RELEASED) {
    /*Increase the button width*/
    lv_coord_t width = lv_obj_get_width(btn);
    lv_obj_set_width(btn, width + 20);
  }
}

/**
 * Called when a new option is chosen in the drop down list
 * @param ddlist pointer to the drop down list
 * @param event the triggering event
 * @return LV_RES_OK because the object is not deleted in this function
 */
static  void ddlist_event_cb(lv_obj_t * ddlist, lv_event_t event)
{
  if(event == LV_EVENT_VALUE_CHANGED) {
    uint16_t opt = lv_ddlist_get_selected(ddlist);            /*Get the id of selected option*/

    lv_slider_set_value(slider, (opt * 100) / 4, true);       /*Modify the slider value according to the selection*/
  }

}

void demo(void) {
  /********************
   * CREATE A SCREEN
   *******************/
  /* Create a new screen and load it
   * Screen can be created from any type object type
   * Now a Page is used which is an objects with scrollable content*/
  lv_obj_t * scr = lv_page_create(NULL, NULL);
  lv_disp_load_scr(scr);

  /****************
   * ADD A TITLE
   ****************/
  lv_obj_t * label = lv_label_create(scr, NULL); /*First parameters (scr) is the parent*/
  lv_label_set_text(label, "Object usage demo");  /*Set the text*/
  lv_obj_set_x(label, 10);                        /*Set the x coordinate*/

  /***********************
   * CREATE TWO BUTTONS
   ***********************/
  /*Create a button*/
  lv_obj_t * btn1 = lv_btn_create(lv_disp_get_scr_act(NULL), NULL);         /*Create a button on the currently loaded screen*/
  lv_obj_set_event_cb(btn1, btn_event_cb);                                  /*Set function to be called when the button is released*/
  lv_obj_set_height(btn1, 30);
  lv_obj_align(btn1, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);               /*Align below the label*/

  /*Create a label on the button (the 'label' variable can be reused)*/
  label = lv_label_create(btn1, NULL);
  lv_label_set_text(label, "Ok");

  /*Copy the previous button*/
  lv_obj_t * btn2 = lv_btn_create(scr, btn1);                 /*Second parameter is an object to copy*/
  lv_obj_align(btn2, btn1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);    /*Align next to the prev. button.*/

  /*Create a label on the button*/
  label = lv_label_create(btn2, NULL);
  lv_label_set_text(label, "Lookup up");

  /****************
   * ADD A SLIDER
   ****************/
  slider = lv_slider_create(scr, NULL);                            /*Create a slider*/
  lv_obj_set_size(slider, lv_obj_get_width(scr)  / 3, LV_DPI / 3);            /*Set the size*/
  lv_obj_align(slider, btn1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);                /*Align below the first button*/
  lv_slider_set_value(slider, 30, false);                                            /*Set the current value*/

  /***********************
   * ADD A DROP DOWN LIST
   ************************/
  lv_obj_t * ddlist = lv_ddlist_create(scr, NULL);                     /*Create a drop down list*/
  lv_obj_align(ddlist, slider, LV_ALIGN_OUT_RIGHT_TOP, 50, 0);         /*Align next to the slider*/
  lv_obj_set_top(ddlist, true);                                        /*Enable to be on the top when clicked*/
  lv_ddlist_set_options(ddlist, "None\nLittle\nHalf\nA lot\nAll");     /*Set the options*/
  lv_obj_set_event_cb(ddlist, ddlist_event_cb);                        /*Set function to call on new option is chosen*/

  /****************
   * CREATE A CHART
   ****************/
  lv_obj_t * chart = lv_chart_create(scr, NULL);                         /*Create the chart*/
  lv_obj_set_size(chart, lv_obj_get_width(scr) / 2, lv_obj_get_width(scr) / 4);   /*Set the size*/
  lv_obj_align(chart, slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 50);                   /*Align below the slider*/
  lv_chart_set_series_width(chart, 3);                                            /*Set the line width*/

  /*Add a RED data series and set some points*/
  lv_chart_series_t * dl1 = lv_chart_add_series(chart, LV_COLOR_RED);
  lv_chart_set_next(chart, dl1, 10);
  lv_chart_set_next(chart, dl1, 25);
  lv_chart_set_next(chart, dl1, 45);
  lv_chart_set_next(chart, dl1, 80);

  /*Add a BLUE data series and set some points*/
  lv_chart_series_t * dl2 = lv_chart_add_series(chart, lv_color_make(0x40, 0x70, 0xC0));
  lv_chart_set_next(chart, dl2, 10);
  lv_chart_set_next(chart, dl2, 25);
  lv_chart_set_next(chart, dl2, 45);
  lv_chart_set_next(chart, dl2, 80);
  lv_chart_set_next(chart, dl2, 75);
  lv_chart_set_next(chart, dl2, 505);

}


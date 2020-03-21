//
// Created by sl on 21.03.2020.
//

#include "lvgl.h"

static lv_obj_t * scr;
lv_obj_t * label;

void construct_hello_screen(void) {
  /*Create a screen*/
  scr = lv_obj_create(NULL, NULL);
  lv_scr_load(scr);
  label = lv_label_create(scr, NULL);
  lv_label_set_text(label, "Yao-GUAI weather station");
  lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 4, 4);
}

void redraw_hello_screen(void) {

}

void destroy_hello_screen(void) {
  lv_obj_del(scr);
}
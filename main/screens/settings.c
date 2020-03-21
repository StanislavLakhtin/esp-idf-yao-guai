//
// Created by sl on 21.03.2020.
//

#include "lvgl.h"
#include "screens.h"

static void group_focus_cb(lv_group_t * group);

static lv_obj_t * setting_win;
static lv_group_t * g;

void construct_settings_screen(void) {
  setting_win = lv_win_create(lv_scr_act(), NULL);
  lv_win_set_title(setting_win, "Settings");
  g = lv_group_create();
  lv_group_set_focus_cb(g, group_focus_cb);
  /*Add control button to the header*/
  lv_obj_t * close_btn = lv_win_add_btn(setting_win, LV_SYMBOL_CLOSE);
  lv_obj_set_event_cb(close_btn, lv_win_close_event);
  lv_group_add_obj(g, close_btn);
  lv_indev_set_group(encoder_indev, g);
}

static void group_focus_cb(lv_group_t * group)
{
  lv_obj_t * f = lv_group_get_focused(group);
  if(f != setting_win) lv_win_focus(setting_win, f, LV_ANIM_ON);
}

void lv_win_close_event(lv_obj_t * obj, lv_event_t event) {
  switch (event) {
    case LV_EVENT_PRESSED:
      printf("Pressed\n");
      break;

    case LV_EVENT_SHORT_CLICKED:
      printf("Short clicked\n");
      break;

    case LV_EVENT_CLICKED:
      printf("Clicked\n");
      break;

    case LV_EVENT_LONG_PRESSED:
      printf("Long press\n");
      break;

    case LV_EVENT_LONG_PRESSED_REPEAT:
      printf("Long press repeat\n");
      break;

    case LV_EVENT_RELEASED:
      printf("Released\n");
      break;

    default:
      printf("Unknown event\n");
  }
}
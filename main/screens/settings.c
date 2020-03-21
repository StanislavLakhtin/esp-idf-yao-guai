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
  /* Add EXIT control button to the header */
  lv_obj_t * close_btn = lv_win_add_btn(setting_win, LV_SYMBOL_CLOSE);
  lv_obj_set_event_cb(close_btn, event_cb);
  lv_group_add_obj(g, close_btn);
  lv_indev_set_group(encoder_indev, g);
  lv_obj_t * obj;
  obj = lv_spinbox_create(setting_win, NULL);
  //lv_obj_set_event_cb(obj, event_cb);
  lv_spinbox_set_digit_format(obj, 5, 2);
  lv_group_add_obj(g, obj);
}

static void group_focus_cb(lv_group_t * group)
{
  lv_obj_t * f = lv_group_get_focused(group);
  if(f != setting_win) lv_win_focus(setting_win, f, LV_ANIM_ON);
}

void event_cb(lv_obj_t * obj, lv_event_t event) {
  switch (event) {
    case LV_KEY_ENTER:
      printf("ENTER\n");
      break;

    case LV_KEY_LEFT:
      printf("LEFT\n");
      break;

    case LV_KEY_RIGHT:
      printf("RIGHT\n");
      break;

    case LV_KEY_NEXT:
      printf("NEXT\n");
      break;

    case LV_KEY_PREV:
      printf("PREV\n");
      break;

    case LV_INDEV_STATE_PR:
      printf("LV_INDEV_STATE_PR\n");
      break;

    default:
      printf("Unknown event %02x \n", event);
  }
}
//
// Created by sl on 28.03.2020.
//

#include "show_scanned_ap.h"

static lv_obj_t *screen = NULL;
static lv_group_t *g = NULL;

static void group_focus_cb(lv_group_t *group);

void construct_scanned_aps_screen(void *arg) {
  lv_obj_t *obj = NULL;
  if (screen != NULL)
    return; // is already constructed

  screen = lv_obj_create(NULL, NULL);
  lv_scr_load(screen);
  obj = lv_win_create(lv_scr_act(), NULL);
  lv_win_set_title(obj, "Nearest WiFi AP's");

  g = lv_group_create();
  lv_group_set_focus_cb(g, group_focus_cb);
  lv_indev_set_group(encoder_indev, g);

  lv_obj_t *_btn = lv_win_add_btn(obj, LV_SYMBOL_HOME);
  lv_obj_set_event_cb(_btn, NULL);
  lv_group_add_obj(g, _btn);
  lv_indev_set_group(encoder_indev, g);
  _btn = lv_win_add_btn(obj, LV_SYMBOL_SETTINGS);
  lv_obj_set_event_cb(_btn, NULL);
  lv_group_add_obj(g, _btn);
  lv_indev_set_group(encoder_indev, g);
}

static void group_focus_cb(lv_group_t *group) {
  lv_obj_t *f_obj = lv_group_get_focused(group);
  if (f_obj != screen) lv_win_focus(screen, f_obj, LV_ANIM_ON);
}

ui_screen_signal_t do_action_scanned_aps_screen(void) {
  return RepeatMyselfEvent;
}

void destroy_scanned_aps_screen(void) {
  lv_obj_del(screen);
  screen = NULL;
}
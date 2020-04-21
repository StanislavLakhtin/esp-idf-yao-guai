//
// Created by sl on 21.03.2020.
//

#include "core/ui_loop.h"

static lv_obj_t * screen;
static lv_group_t * g;

static const char * TAG = "Hello Screen";

static void call_settings_event_cb(lv_obj_t * obj, lv_event_t event);
static void group_focus_cb(lv_group_t * group);

typedef enum {
  DoNothing,
  CallSettings
} internal_event_t;

internal_event_t int_event = DoNothing;

void construct_hello_screen(void * arg) {
  /*Create a screen*/
  if ( screen != NULL)
    return; // it is already constructed

  screen = lv_obj_create(NULL, NULL);
  lv_scr_load(screen);
  lv_obj_t * label = lv_label_create(screen, NULL);
  lv_label_set_text(label, "Yao-GUAI weather station");
  lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 4);
  lv_obj_set_event_cb(screen, call_settings_event_cb);

  g = lv_group_create();
  lv_indev_set_group(encoder_indev, g);
  lv_group_set_focus_cb(g, group_focus_cb);

  lv_obj_t * _btn = lv_btn_create(screen, NULL);
  lv_obj_set_event_cb(_btn, call_settings_event_cb);
  lv_obj_align(_btn, screen, LV_ALIGN_CENTER, 0 ,0 );
  label = lv_label_create(_btn, NULL);
  lv_label_set_text(label, "Settings");
  lv_group_add_obj(g, _btn);

  int_event = DoNothing;
}

static void group_focus_cb(lv_group_t * group) {
  //lv_obj_t * f_obj = lv_group_get_focused(group);
  //if(f_obj != screen) lv_win_focus(screen, f_obj, LV_ANIM_ON);
}

static void call_settings_event_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_LONG_PRESSED) {
    ESP_LOGI(TAG, "LV_EVENT_LONG_PRESSED");
    int_event = CallSettings;
  } else {
    ESP_LOGI(TAG, "LV_EVENT %d", event);
  }
}

ui_screen_signal_t do_action_hello_screen(void) {
  return (int_event == CallSettings) ? SettingsEvent : RepeatMyselfEvent;
}

void destroy_hello_screen(void) {
  lv_group_del(g);
  lv_obj_del(screen);
  screen = NULL;
}
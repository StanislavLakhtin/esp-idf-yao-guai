//
// Created by sl on 28.03.2020.
//

#include "show_scanned_ap.h"

static const char * TAG = "ap list";

static lv_obj_t *screen = NULL;
static lv_group_t *g = NULL;

static void group_focus_cb(lv_group_t *group);
static void call_home_event_cb(lv_obj_t * obj, lv_event_t event);
static void call_settings_event_cb(lv_obj_t * obj, lv_event_t event);

void construct_scanned_aps_screen(void *arg) {
  lv_obj_t *obj = NULL;
  if (screen != NULL)
    return; // It is already constructed

  ESP_LOGI(TAG, "Construct show scanned AP list");

  screen = lv_obj_create(NULL, NULL);
  lv_scr_load(screen);

  obj = lv_label_create(screen, NULL);
  lv_label_set_long_mode(obj, LV_LABEL_LONG_BREAK);
  lv_label_set_align(obj, LV_ALIGN_CENTER);
  lv_label_set_text(obj, "Wait a little. WiFi scanning...");

  xEventGroupSetBits(xEvents, WIFI_SCAN);
  EventBits_t currentEvents;
  do {
    currentEvents = xEventGroupGetBits(xEvents);
  } while (currentEvents & WIFI_SCAN);

  lv_obj_del(obj);

  // --------------------------------------

  obj = lv_win_create(lv_scr_act(), NULL);
  lv_win_set_title(obj, "Nearest WiFi AP's");

  g = lv_group_create();
  //lv_group_set_focus_cb(g, group_focus_cb);
  lv_indev_set_group(encoder_indev, g);

  lv_obj_t *_btn = lv_win_add_btn(obj, LV_SYMBOL_HOME);
  lv_obj_set_event_cb(_btn, call_home_event_cb);
  lv_group_add_obj(g, _btn);

  _btn = lv_win_add_btn(obj, LV_SYMBOL_SETTINGS);
  lv_obj_set_event_cb(_btn, call_settings_event_cb);
  lv_group_add_obj(g, _btn);

  lv_obj_t *lst = NULL;
  lst = lv_list_create(obj, NULL);
  //lv_obj_align(lst, obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

  for (int i = 0; i < ap_cnt; i++) {
    wifi_ap_record_t *ap = &ap_info[i];
    const char *txt = (const char *) ap->ssid;
    ESP_LOGI(TAG, "add wifi button '%s'", txt);
    lv_list_add_btn(lst, LV_SYMBOL_WIFI, txt);
  }
  lv_group_add_obj(g, lst);
}

static void group_focus_cb(lv_group_t *group) {
  lv_obj_t *f_obj = lv_group_get_focused(group);
  if (f_obj != screen) lv_win_focus(screen, f_obj, LV_ANIM_ON);
}

ui_screen_signal_t do_action_scanned_aps_screen(void) {
  return RepeatMyselfEvent;
}

static void call_home_event_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    ESP_LOGI(TAG, "clicked call_home_event_cb");
    int_show_scanned_ap_signal = ReturnToHomeSignal;
  }
}

static void call_settings_event_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    ESP_LOGI(TAG, "clicked call_settings_event_cb");
    int_show_scanned_ap_signal = ReturnToSettingsSignal;
  }
}

void destroy_scanned_aps_screen(void) {
  lv_group_del(g);
  lv_obj_del(screen);
  screen = NULL;
}
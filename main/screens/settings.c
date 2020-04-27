//
// Created by sl on 21.03.2020.
//

#include "screens.h"

static const char * TAG = "settings";

static void wifi_scan_event_cb(lv_obj_t * obj, lv_event_t event);
static void edit_ap_event_cb(lv_obj_t * obj, lv_event_t event);
static void scan_1wire_event_cb(lv_obj_t * obj, lv_event_t event);
static void group_focus_cb(lv_group_t * group);

static lv_obj_t * screen = NULL;
static lv_obj_t * setting_win = NULL;
static lv_group_t * g = NULL;

void construct_settings_screen(void * arg) {
  if ( screen != NULL)
    return; // it is already constructed

  screen = lv_obj_create(NULL, NULL);
  lv_scr_load(screen);
  setting_win = lv_win_create(lv_scr_act(), NULL);
  lv_win_set_title(setting_win, "Settings");

  g = lv_group_create();
  lv_indev_set_group(encoder_indev, g);

  /* Add EXIT control button to the header */
  lv_obj_t * _btn, *obj;

  _btn = lv_win_add_btn(setting_win, LV_SYMBOL_HOME);
  lv_obj_set_event_cb(_btn, close_event_cb);
  lv_group_add_obj(g, _btn);

  obj = lv_list_create(setting_win, NULL);
  lv_group_add_obj(g, obj);
  lv_obj_align(obj, setting_win, LV_ALIGN_CENTER, 0, 0);

  _btn = lv_list_add_btn(obj, LV_SYMBOL_WIFI, "Scan WiFi");
  lv_obj_set_height(_btn, 32);
  lv_obj_set_event_cb(_btn, wifi_scan_event_cb);

  _btn = lv_list_add_btn(obj, LV_SYMBOL_EDIT, "Edit known APs");
  lv_obj_set_event_cb(_btn, edit_ap_event_cb);

  _btn = lv_list_add_btn(obj, LV_SYMBOL_REFRESH, "1-wire scan");
  lv_obj_set_event_cb(_btn, scan_1wire_event_cb);

  int_screen_event = DoNothing;
}

ui_screen_signal_t do_action_settings_screen(void) {
  ui_screen_signal_t result = RepeatMyselfEvent;
  switch (int_screen_event) {
    case ReturnToHOMEScreen:
      result = CallHomeScreenEvent;
      break;
    case ShowNearestWiFiScreen:
      ESP_LOGI(TAG, "Show nearest AP's");
      result = ShowNearestWiFiAPEvent;
      break;
    default:
      result = RepeatMyselfEvent;
      break;
  }
  return result;
}

void destroy_settings_screen(void) {
  lv_group_del(g);
  lv_obj_del(screen);
  screen = NULL;
}

static void group_focus_cb(lv_group_t * group) {
  lv_obj_t * f_obj = lv_group_get_focused(group);
  if(f_obj != setting_win) lv_win_focus(setting_win, f_obj, LV_ANIM_ON);
}

static lv_obj_t * mbox;

static void wifi_scan_event_cb(lv_obj_t * obj, lv_event_t event) {
  ESP_LOGI(TAG, "wifi_scan_event_cb");
  if (event == LV_EVENT_CLICKED) {
    int_screen_event = ShowNearestWiFiScreen;
  }
}

static void edit_ap_event_cb(lv_obj_t * obj, lv_event_t event) {
  ESP_LOGI(TAG, "edit_ap_event_cb");
}

static void scan_1wire_event_cb(lv_obj_t * obj, lv_event_t event) {
  ESP_LOGI(TAG, "scan_1wire_event_cb");
}

void close_event_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    ESP_LOGI(TAG, "LV_EVENT_PRESSED");
    int_screen_event = ReturnToHOMEScreen;
  } else {
    ESP_LOGI(TAG, "LV_EVENT %d", event);
  }
}
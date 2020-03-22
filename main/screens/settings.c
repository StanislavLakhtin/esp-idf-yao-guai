//
// Created by sl on 21.03.2020.
//

#include "esp_log.h"
#include "lvgl.h"
#include "screens.h"

static const char * TAG = "settings";

static void group_focus_cb(lv_group_t * group);
static void wifi_scan_event_cb(lv_obj_t * obj, lv_event_t event);
static void edit_ap_event_cb(lv_obj_t * obj, lv_event_t event);

    static lv_obj_t * setting_win = NULL;
static lv_group_t * g = NULL;

void construct_settings_screen(void) {
  if ( setting_win != NULL)
    return; // is already constructed
  setting_win = lv_win_create(lv_scr_act(), NULL);
  lv_win_set_title(setting_win, "Settings");
  g = lv_group_create();
  lv_group_set_focus_cb(g, group_focus_cb);
  /* Add EXIT control button to the header */
  lv_obj_t * _btn = lv_win_add_btn(setting_win, LV_SYMBOL_CLOSE);
  lv_obj_set_event_cb(_btn, event_cb);
  lv_group_add_obj(g, _btn);
  lv_indev_set_group(encoder_indev, g);
  lv_obj_t * obj;
  obj = lv_list_create(setting_win, NULL);
  lv_obj_align(obj, setting_win, LV_ALIGN_CENTER, 0, 0);
  _btn = lv_list_add_btn(obj, LV_SYMBOL_WIFI, "Scan WiFi");
  lv_obj_set_height(_btn, 32);
  lv_obj_set_event_cb(_btn, wifi_scan_event_cb);
  _btn = lv_list_add_btn(obj, LV_SYMBOL_EDIT, "Edit known APs");
  lv_obj_set_event_cb(_btn, edit_ap_event_cb);
  _btn = lv_list_add_btn(obj, LV_SYMBOL_DRIVE, "SDCard");
  lv_obj_set_event_cb(_btn, edit_ap_event_cb);
  lv_group_add_obj(g, obj);
}

static void group_focus_cb(lv_group_t * group)
{
  lv_obj_t * f = lv_group_get_focused(group);
  if(f != setting_win) lv_win_focus(setting_win, f, LV_ANIM_ON);
}

static void wifi_scan_event_cb(lv_obj_t * obj, lv_event_t event) {
  ESP_LOGI(TAG, "wifi_scan_event_cb");
  //event_cb(obj, event);
}

static void edit_ap_event_cb(lv_obj_t * obj, lv_event_t event) {
  ESP_LOGI(TAG, "edit_ap_event_cb");
  //event_cb(obj, event);
}

void event_cb(lv_obj_t * obj, lv_event_t event) {
  switch (event) {
    case LV_EVENT_PRESSED:
      printf("LV_EVENT_PRESSED\n");
      break;

    case LV_EVENT_SHORT_CLICKED:
      printf("LV_EVENT_SHORT_CLICKED\n");
      break;

    case LV_EVENT_LONG_PRESSED:
      printf("LV_EVENT_LONG_PRESSED\n");
      break;

    case LV_EVENT_RELEASED:
      printf("LV_EVENT_RELEASED\n");
      break;

    case LV_EVENT_CLICKED:
      printf("LV_EVENT_CLICKED\n");
      break;

    /*case LV_EVENT_FOCUSED:
      printf("LV_EVENT_FOCUSED\n");
      break;

    case LV_EVENT_DEFOCUSED:
      printf("LV_EVENT_DEFOCUSED\n");
      break;

    default:
      printf("Unknown event 0x%02x (%02d) \n", event, event);*/
  }
}
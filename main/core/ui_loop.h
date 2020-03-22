#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_H

typedef enum {
  UNKNOWN,
  SETTINGS,
  SHOW_SCANNED_APS
} ui_screen_state_t;

lv_obj_t * current_display;
ui_screen_state_t ui_screen;

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_H
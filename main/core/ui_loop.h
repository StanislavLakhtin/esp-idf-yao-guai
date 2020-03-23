#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_H

#include "yao-guai.h"

#define CALL_IF_NOT_NULL(fn, arg) do {\
  if (fn != NULL)\
    fn(arg);\
  } while (0)\

typedef enum {
  UNDEFINED,
  HELLO,
  SETTINGS,
  SHOW_SCANNED_APS,
  SHOW_
} ui_screen_state_t;

typedef enum {
  RepeatMyselfEvent,
  SettingsEvent,
  ScanWifiEvent,
  ConnectToApEvent
} ui_screen_signal_t;

typedef ui_screen_signal_t (* ui_transition_action_fptr_t)(void);
typedef void (* ui_construct_ui_fptr_t)(void *);
typedef void (* ui_deconstruct_ui_fptr_t)(void);

typedef struct {
  ui_screen_state_t from;
  ui_screen_state_t to;
  ui_screen_signal_t signal;
  ui_construct_ui_fptr_t do_construct;
  ui_transition_action_fptr_t do_action;
  ui_deconstruct_ui_fptr_t do_deconstruct;
} ui_transition_t;

ui_screen_state_t ui_screen;

lv_indev_drv_t encoder_drv;
lv_indev_t * encoder_indev;

#ifdef __cplusplus
extern "C"
{
#endif

void construct_hello_screen(void * arg);
ui_screen_signal_t do_action_hello_screen(void);
void destroy_hello_screen(void);


void construct_settings_screen(void * arg);
ui_screen_signal_t do_action_settings_screen(void);
void destroy_settings_screen(void);

#ifdef __cplusplus
}
#endif

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_UI_H
//
// Created by sl on 21.03.2020.
//

#ifndef YAO_GUAI_SCREENS_H
#define YAO_GUAI_SCREENS_H

#include "yao-guai.h"

typedef enum {
  DoNothing,
  ReturnToHOMEScreen,
  ShowNearestWiFiScreen
} internal_screen_event_t;

internal_screen_event_t int_screen_event;

#ifdef __cplusplus
extern "C"
{
#endif

void close_event_cb (lv_obj_t * obj, lv_event_t event);

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_SCREENS_H

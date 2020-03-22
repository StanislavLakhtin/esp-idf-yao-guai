//
// Created by sl on 21.03.2020.
//

#ifndef YAO_GUAI_SCREENS_H
#define YAO_GUAI_SCREENS_H

#include "yao-guai.h"

lv_indev_drv_t encoder_drv;
lv_indev_t * encoder_indev;

#ifdef __cplusplus
extern "C"
{
#endif

void construct_settings_screen(void);
void construct_scanned_aps_screen(void);

void event_cb (lv_obj_t * obj, lv_event_t event);

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_SCREENS_H

//
// Created by sl on 21.03.2020.
//

#ifndef YAO_GUAI_SCREENS_H
#define YAO_GUAI_SCREENS_H

lv_indev_drv_t encoder_drv;
lv_indev_t * encoder_indev;

#ifdef __cplusplus
extern "C"
{
#endif

void construct_settings_screen(void);

void lv_win_close_event (lv_obj_t * obj, lv_event_t event);

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_SCREENS_H

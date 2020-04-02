//
// Created by sl on 28.03.2020.
//

#ifndef YAO_GUAI_SHOW_SCANNED_AP_H
#define YAO_GUAI_SHOW_SCANNED_AP_H

#include "yao-guai.h"

typedef enum {
  DoNothingSignal,
  ReturnToHomeSignal,
  ReturnToSettingsSignal,
  ConnectToChosenApScreenSignal
} show_scanned_ap_signal_t;

show_scanned_ap_signal_t int_show_scanned_ap_signal;

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_SHOW_SCANNED_AP_H

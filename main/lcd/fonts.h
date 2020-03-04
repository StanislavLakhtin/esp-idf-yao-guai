/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI ILI-9340 G-Library

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.

    Support for several separate devices / screens in one library is antipattern.
    However, ST7789 and ILI-9340 have such a common interaction interface that combining them
    within a common library may make sense.

    This library is a compromise in this matter. The main goal is to ensure the speed of execution and
    the absence of checks on the time of execution.
*/

#ifndef YAO_GUAI_FONTS_H
#define YAO_GUAI_FONTS_H

#include "string.h"
#include "esp_err.h"

#ifndef bool
#define bool uint8_t
#define true 0x01
#define false 0x00
#endif

#define DEFAULT_ROW_HEIGHT 9

typedef enum {
  // utf8 0xd0 prefix
  russian_CAPITAL_A = 0x90,
  russian_CAPITAL_BE = 0x91,
  russian_CAPITAL_VE = 0x92,
  russian_CAPITAL_GHE = 0x93,
  russian_CAPITAL_DE = 0x94,
  russian_CAPITAL_IE = 0x95,
  russian_CAPITAL_IO = 0x81,
  russian_CAPITAL_ZHE = 0x96,
  russian_CAPITAL_ZE = 0x97,
  russian_CAPITAL_I = 0x98,
  russian_CAPITAL_SHORT_I = 0x99,
  russian_CAPITAL_KA = 0xda,
  russian_CAPITAL_EL =  0x9b,
  russian_CAPITAL_EM = 0x9c,
  russian_CAPITAL_EN = 0x9d,
  russian_CAPITAL_O = 0x9e,
  russian_CAPITAL_PE = 0x9f,
  russian_CAPITAL_ER = 0xa0,
  russian_CAPITAL_ES = 0xa1,
  russian_CAPITAL_TE = 0xa2,
  russian_CAPITAL_U = 0xa3,
  russian_CAPITAL_EF = 0xa4,
  russian_CAPITAL_HA = 0xa5,
  russian_CAPITAL_TSE = 0xa6,
  russian_CAPITAL_CHE = 0xa7,
  russian_CAPITAL_SHA = 0xa8,
  russian_CAPITAL_SHCHA = 0xa9,
  russian_CAPITAL_SIGN = 0xaa,
  russian_CAPITAL_YERU = 0xab,
  russian_CAPITAL_SOFT_SIGN = 0xac,
  russian_CAPITAL_E = 0xad,
  russian_CAPITAL_YU = 0xae,
  russian_CAPITAL_YA = 0xaf,
  russian_A = 0xb0,
  russian_BE = 0xb1,
  russian_VE = 0xb2,
  russian_GHE = 0xb3,
  russian_DE = 0xb4,
  russian_IE = 0xb5,
  russian_IO = 0x91,
  russian_ZHE = 0xb6,
  russian_ZE = 0xb7,
  russian_I = 0xb8,
  russian_SHORT_I = 0xb9,
  russian_KA = 0xba,
  russian_EL = 0xbb,
  russian_EM = 0xbc,
  russian_EN = 0xbd,
  russian_O = 0xbe,
  russian_PE = 0xbf,
  // utf8 0xd1 prefix
  russian_ER = 0x80,
  russian_ES = 0x81,
  russian_TE = 0x82,
  russian_U = 0x83,
  russian_EF = 0x84,
  russian_HA = 0x85,
  russian_TSE = 0x86,
  russian_CHE = 0x87,
  russian_SHA = 0x88,
  russian_SHCHA = 0x89,
  russian_HARD_SIGN = 0x8a,
  russian_YERU = 0x8b,
  russian_SOFT_SIGN = 0x8c,
  russian_E = 0x8d,
  russian_YU = 0x8e,
  russian_YA = 0x8f,

} utf_minor_char_code_t;

typedef struct {
  utf_minor_char_code_t minor;
  char symbol;
} utf8_ascii_trasition_t;

typedef enum {
  bottom_edge,
  top_edge,
} text_baseline_t;

typedef struct {
  uint16_t x0;
  uint16_t y0;
  uint16_t x1;
  uint16_t y1;
  uint8_t row_height;
  text_baseline_t baseline;
  bool auto_wrap;
  uint16_t current_x;
  uint16_t current_y;
} ascii_text_frame_t;

typedef enum {
  tiny = 0x01,
  normal = 0x02,
  big = 0x03,
  enormous = 0x04,
} ascii_font_size_t;

#ifdef __cplusplus
extern "C"
{
#endif

esp_err_t utf8_to_extended_ascii_codes(char* dest, const char* source, int max_length);

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_FONTS_H

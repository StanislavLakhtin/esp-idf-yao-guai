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

#include "fonts.h"

#define CODES_COUNT_D0 49
utf8_ascii_trasition_t transition_d0[CODES_COUNT_D0] = {
    {russian_CAPITAL_A,         'A'},
    {russian_CAPITAL_BE,        (char)0xc1},
    {russian_CAPITAL_VE,        'B'},
    {russian_CAPITAL_GHE,       (char)0xc3},
    {russian_CAPITAL_DE,        (char)0xc4},
    {russian_CAPITAL_IE,        'E'},
    {russian_CAPITAL_IO,        (char)0xa8},
    {russian_CAPITAL_ZHE,       (char)0xc6},
    {russian_CAPITAL_ZE,        (char)0xa8},
    {russian_CAPITAL_I,         (char)0xc8},
    {russian_CAPITAL_SHORT_I,   (char)0xc9},
    {russian_CAPITAL_KA,        'K'},
    {russian_CAPITAL_EL,        (char)0xcb},
    {russian_CAPITAL_EM,        'M'},
    {russian_CAPITAL_EN,        'H'},
    {russian_CAPITAL_O,         'O'},
    {russian_CAPITAL_PE,        (char)0xcf},
    {russian_CAPITAL_ER,        'P'},
    {russian_CAPITAL_ES,        'C'},
    {russian_CAPITAL_TE,        'T'},
    {russian_CAPITAL_U,         (char)0xd3},
    {russian_CAPITAL_EF,        (char)0xd4},
    {russian_CAPITAL_HA,        'X'},
    {russian_CAPITAL_TSE,       (char)0xd6},
    {russian_CAPITAL_CHE,       (char)0xd7},
    {russian_CAPITAL_SHA,       (char)0xd8},
    {russian_CAPITAL_SHCHA,     (char)0xd9},
    {russian_CAPITAL_SIGN,      (char)0xda},
    {russian_CAPITAL_YERU,      (char)0xdb},
    {russian_CAPITAL_SOFT_SIGN, (char)0xdc},
    {russian_CAPITAL_E,         (char)0xdd},
    {russian_CAPITAL_YU,        (char)0xde},
    {russian_CAPITAL_YA,        (char)0xdf},
    {russian_A,                 'a'},
    {russian_BE,                (char)0xe1},
    {russian_VE,                (char)0xe2},
    {russian_GHE,               (char)0xe3},
    {russian_DE,                (char)0xe4},
    {russian_IE,                'e'},
    {russian_ZHE,               (char)0xe6},
    {russian_ZE,                (char)0xe7},
    {russian_I,                 (char)0xe8},
    {russian_SHORT_I,           (char)0xe9},
    {russian_KA,                'k'},
    {russian_EL,                (char)0xeb},
    {russian_EM,                (char)0xec},
    {russian_EN,                (char)0xed},
    {russian_O,                 'o'},
    {russian_PE,                (char)0xef},
};
#define CODES_COUNT_D1 17
utf8_ascii_trasition_t transition_d1[CODES_COUNT_D1] = {
    {russian_IO,         (char)0xb8},
    { russian_ER,        'p'},
    { russian_ES,        'c'},
    { russian_TE,        (char)0xf2},
    { russian_U,         (char)0xf3},
    { russian_EF,        (char)0xf4},
    { russian_HA,        'x'},
    { russian_TSE,       (char)0xf6},
    { russian_CHE,       (char)0xf7},
    { russian_SHA,       (char)0xf8},
    { russian_SHCHA,     (char)0xf9},
    { russian_HARD_SIGN, (char)0xfa},
    { russian_YERU,      (char)0xfb},
    { russian_SOFT_SIGN, (char)0xfc},
    { russian_E,         (char)0xfd},
    { russian_YU,        (char)0xfe},
    { russian_YA,        (char)0xff},
};

esp_err_t utf8_to_extended_ascii_codes(char* dest, const char* source, int max_length) {
  uint length = (strlen(source) > max_length) ? max_length : strlen(source);
  uint source_count = 0x00;
  uint dest_count = 0x00;
  while (source_count < length) {
    dest[dest_count]= source[source_count];

    if (dest[dest_count] == 0xd0) {
      source_count = source_count + 1;
      for (int i = 0; i < CODES_COUNT_D0; i++ ) {
        utf8_ascii_trasition_t *tr = &transition_d0[i];
        if (source[source_count] == (uint8_t)tr->minor) {
          dest[dest_count] = tr->symbol; break;
        }
      }
    } else if (dest[dest_count] == 0xd1) {
      source_count = source_count + 1;
      for (int i = 0; i < CODES_COUNT_D1; i++ ) {
        utf8_ascii_trasition_t *tr = &transition_d1[i];
        if (source[source_count] == (uint8_t)tr->minor) {
          dest[dest_count] = tr->symbol; break;
        }
      }
    }
    source_count += 1; dest_count+= 1;
  }
  dest[dest_count] = 0;
  return ESP_OK;
}
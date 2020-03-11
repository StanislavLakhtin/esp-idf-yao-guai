/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI UI FSM

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

#include "ui_fsm.h"

enum ret_codes_t ui_settings_view( void ) {
  ESP_LOGI(TAG, "fill black");
  color_t bg , c;
  set_color(bg, 0,0,0);
  fill_rect(lcd_dev, 0, 0, lcd_dev->width,  lcd_dev->height, &bg);
  set_color(c, MAX_INTENSITY,MAX_INTENSITY, MAX_INTENSITY);
  ascii_text_frame_t ascii_frame = {
      .x0 = 4,
      .y0 = 4,
      .x1 = lcd_dev->width - 4,
      .y1 = lcd_dev->height -4,
      .auto_wrap = true,
      .row_height = DEFAULT_ROW_HEIGHT,
      .current_x = 4,
      .current_y = 4
  };
  for (uint16_t i = 0; i < 0xff; i++) {
    char ch = i;
    uint8_t colors[3];
    esp_fill_random(colors, 3);
    set_color(c, colors[0] / 4,colors[1] / 4, colors[2] / 4);
    draw_monospace_text(lcd_dev, &ascii_frame, &ch, 1, &c, &bg, normal, normal);
  }
  set_color(bg, 0,0, 0);
  uint8_t min = 0x00;
  uint8_t max = 0xff;
  uint8_t med = max / 2;
  lcd_dev->write_cmnd(TFT_DISBR);
  lcd_dev->write_data(&min, 1);
  LISTEN_IO_MS(5000);
  lcd_dev->write_cmnd(TFT_DISBR);
  lcd_dev->write_data(&max, 1);
  LISTEN_IO_MS(5000);
  lcd_dev->write_cmnd(TFT_DISBR);
  lcd_dev->write_data(&med, 1);
  LISTEN_IO_MS(5000);
  fill_rect(lcd_dev, 0, 0, lcd_dev->width,  lcd_dev->height, &bg);
  ascii_frame.current_x = 4;
  ascii_frame.current_y = 4;
  char * str = "В чащах юга жил был цитрус?\nДа, но фальшивый экземпляр!\nСъешь же ещё этих мягких французских булок, да выпей чаю.\nЁмаё! (для буквы ё)";
  draw_monospace_text(lcd_dev, &ascii_frame, str, strlen(str), &c, &bg, normal, normal);
  LISTEN_IO_MS(5000);
  fill_rect(lcd_dev, 0, 0, lcd_dev->width,  lcd_dev->height, &bg);
  ascii_frame.current_x = 4;
  ascii_frame.current_y = 4;
  draw_monospace_text(lcd_dev, &ascii_frame, "Размеры 1x1\n", strlen(str), &c, &bg, tiny, tiny);
  draw_monospace_text(lcd_dev, &ascii_frame, "Размеры 1x2\n", strlen(str), &c, &bg, tiny, normal);
  draw_monospace_text(lcd_dev, &ascii_frame, "Размеры 2x2\n", strlen(str), &c, &bg, normal, normal);
  draw_monospace_text(lcd_dev, &ascii_frame, "Размеры 2x3\n", strlen(str), &c, &bg, normal, big);
  draw_monospace_text(lcd_dev, &ascii_frame, "Размеры 3x3\n", strlen(str), &c, &bg, big, big);
  draw_monospace_text(lcd_dev, &ascii_frame, "Это 3x4\n", strlen(str), &c, &bg, big, big);
  draw_monospace_text(lcd_dev, &ascii_frame, "Даже 4x4!\n", strlen(str), &c, &bg, enormous, enormous);
  draw_monospace_text(lcd_dev, &ascii_frame, "А возможно и странные 1x4. А вдруг?!\n", strlen(str), &c, &bg, tiny, enormous);
  LISTEN_IO_MS(5000);
  fflush(stdout);
  return FSM_OK;
}
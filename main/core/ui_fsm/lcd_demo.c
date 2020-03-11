//
// Created by Stanislav Lakhtin on 11/03/2020.
// view to debug lcd primitives
//

#include "ui_fsm.h"

static void lines_test ( void ) {
  ESP_LOGI(TAG, "lines DEMO Screen");
  color_t bg , c;
  set_color(bg, 0,0,0);
  fill_rect(lcd_dev, 0, 0, lcd_dev->width,  lcd_dev->height, &bg);
  for (int i = 0; i < 300; i ++ ){
    uint16_t x = esp_random() % lcd_dev->width;
    uint16_t y = esp_random() % lcd_dev->height;
    set_color(c, esp_random() % 0xff,esp_random() % 0xff,esp_random() % 0xff);
    draw_fast_h_line(lcd_dev, x, y, 10, &c);
  }
  for (int i = 0; i < 300; i ++ ){
    uint16_t x = esp_random() % lcd_dev->width;
    uint16_t y = esp_random() % lcd_dev->height;
    set_color(c, esp_random() % 0xff,esp_random() % 0xff,esp_random() % 0xff);
    draw_fast_v_line(lcd_dev, x, y, 10, &c);
  }
}

static void rect

void yg_lcd_demo_view( void ) {

  LISTEN_IO_MS(5000);
  set_color(bg, 0,0,0);
  fill_rect(lcd_dev, 0, 0, lcd_dev->width,  lcd_dev->height, &bg);
  for (int i = 0; i < lcd_dev->height / 2; i += 4 ){
    uint16_t x = i;
    uint16_t y = i;
    set_color(c, 0xff - i,0xff - i,0xff - i);
    draw_fast_h_line(lcd_dev, x, y, lcd_dev->width - x * 2, &c);
    draw_fast_v_line(lcd_dev, x, y, lcd_dev->height - y * 2, &c);
    draw_fast_h_line(lcd_dev, x, lcd_dev->height - y, lcd_dev->width - x * 2, &c);
    draw_fast_v_line(lcd_dev, lcd_dev->width - x, y, lcd_dev->height - y * 2, &c);
  }
  LISTEN_IO_MS(5000);
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
    set_color(c, colors[0],colors[1], colors[2]);
    draw_monospace_text(lcd_dev, &ascii_frame, &ch, 1, &c, &bg, normal, normal);
  }
  set_color(bg, 0,0, 0);
  uint8_t min = 0x00;
  uint8_t max = 0xff;
  uint8_t med = max / 2;
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
}
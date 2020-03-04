/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI LCD Driver Example

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
#include <stdio.h>
#include <esp_log.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "yao-guai.h"
#include "lcd/lcd_driver.h"
#include "lcd_spi_driver/esp32_lcd_spi_driver.h"

#define PAUSE(SEC) for (int i = SEC; i > 0; i--) { \
    printf("Restarting in %d seconds...\n", i); \
    delayMs(1000); \
 }

static const char *TAG = "LCD";

// Для определения положения экрана определить соответствующие биты в MADCTL в процессе инициализации

lcd_device_t lcd_dev = {
    .width = 320,
    .height = 240,
    .offset_x = 0,
    .offset_y = 0,
    .write_cmnd = &spi_write_lcd_cmnd,
    .write_data = &spi_write_lcd_data,
    .read_data = &spi_read_lcd_data,
    .set_backlight = &display_set_backlight,
};

esp_err_t test_display(lcd_device_t *dev);

void display_task(void *args) {
  ESP_LOGI(TAG, "Hello world from Yao GUAI [meteo]station.\n");

  spi_master_lcd_init();

  ESP_LOGI(TAG, "  MOSI: %d", GPIO_MOSI);
  ESP_LOGI(TAG, "  SCLK: %d", GPIO_SCLK);
  ESP_LOGI(TAG, "    CS: %d", GPIO_CS);
  ESP_LOGI(TAG, "    DC: %d", GPIO_DC);
  ESP_LOGI(TAG, "   RST: %d", GPIO_RESET);
  ESP_LOGI(TAG, "    BL: %d\n", GPIO_BL);

  ESP_LOGI(TAG, "Detect LCD type\n");

  int id = get_id(&lcd_dev);
  type_lcd_t lcd_detected_type = LCD_TYPE_ILI9340;

  if (id) {
    // none-zero, ST7789 chip detected
    lcd_detected_type = LCD_TYPE_ST7789;
    ESP_LOGI(TAG, "ST7789V detected.\n");
  } else
    ESP_LOGI(TAG, "ILI934x detected.\n");

#ifdef CONFIG_LCD_TYPE_AUTO
  type_lcd_t lcd_type = lcd_detected_type;
#elif defined( CONFIG_LCD_TYPE_ST7789V )
  ESP_LOGI(TAG, "kconfig: force CONFIG_LCD_TYPE_ST7789V.\n");
  type_lcd_t lcd_type = LCD_TYPE_ST7789;
#elif defined( CONFIG_LCD_TYPE_ILI9341 )
  ESP_LOGI(TAG, "kconfig: force CONFIG_LCD_TYPE_ILI9341.\n");
  type_lcd_t lcd_type = LCD_TYPE_ILI9340;
#endif

  lcd_init(&lcd_dev, lcd_type);   // take a note, that you MUST call init only after
  ESP_LOGI(TAG, "Display w:h (%d:%d)", lcd_dev.width, lcd_dev.height);
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  ESP_LOGI(TAG, "This is %s chip with %d CPU cores, WiFi%s%s, ",
           CHIP_NAME,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  ESP_LOGI(TAG, "silicon revision %d, ", chip_info.revision);

  ESP_LOGI(TAG, "%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
  while(1)
    test_display(&lcd_dev);
  esp_restart();
}


esp_err_t test_display(lcd_device_t *dev) {
  PAUSE(1)
  ESP_LOGI(TAG, "fill black");
  color_t bg = {.r = 0x0, .g = 0x0, .b = 0x0};
  color_t c = {.r = MAX, .g = 0x0, .b = 0x0};
  PAUSE(1)
  set_color(bg, 0,0,0);
  fill_rect(dev, 0, 0, dev->width,  dev->height, &bg);
  set_color(c, MAX,MAX, MAX);
  ascii_text_frame_t ascii_frame = {
      .x0 = 4,
      .y0 = 4,
      .x1 = dev->width - 4,
      .y1 = dev->height -4,
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
    draw_monospace_text(dev, &ascii_frame, &ch, 1, &c, &bg, normal, normal);
  }
  set_color(bg, 0,0, 0);
  uint8_t min = 0x00;
  uint8_t max = 0xff;
  uint8_t med = max / 2;
  dev->write_cmnd(TFT_DISBR);
  dev->write_data(&min, 1);
  PAUSE(5);
  dev->write_cmnd(TFT_DISBR);
  dev->write_data(&max, 1);
  PAUSE(5);
  dev->write_cmnd(TFT_DISBR);
  dev->write_data(&med, 1);
  PAUSE(5);
  fill_rect(dev, 0, 0, dev->width,  dev->height, &bg);
  ascii_frame.current_x = 4;
  ascii_frame.current_y = 4;
  char * str = "В чащах юга жил был цитрус?\nДа, но фальшивый экземпляр!\nСъешь же ещё этих мягких французских булок, да выпей чаю.\nЁмаё! (для буквы ё)";
  draw_monospace_text(dev, &ascii_frame, str, strlen(str), &c, &bg, normal, normal);
  PAUSE(5);
  fill_rect(dev, 0, 0, dev->width,  dev->height, &bg);
  ascii_frame.current_x = 4;
  ascii_frame.current_y = 4;
  draw_monospace_text(dev, &ascii_frame, "Размеры 1x1\n", strlen(str), &c, &bg, tiny, tiny);
  draw_monospace_text(dev, &ascii_frame, "Размеры 1x2\n", strlen(str), &c, &bg, tiny, normal);
  draw_monospace_text(dev, &ascii_frame, "Размеры 2x2\n", strlen(str), &c, &bg, normal, normal);
  draw_monospace_text(dev, &ascii_frame, "Размеры 2x3\n", strlen(str), &c, &bg, normal, big);
  draw_monospace_text(dev, &ascii_frame, "Размеры 3x3\n", strlen(str), &c, &bg, big, big);
  draw_monospace_text(dev, &ascii_frame, "Это 3x4\n", strlen(str), &c, &bg, big, big);
  draw_monospace_text(dev, &ascii_frame, "Даже 4x4!\n", strlen(str), &c, &bg, enormous, enormous);
  draw_monospace_text(dev, &ascii_frame, "А возможно и странные 1x4. А вдруг?!\n", strlen(str), &c, &bg, tiny, enormous);
  PAUSE(5);
  fflush(stdout);
  return ESP_OK;
}

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

  ESP_LOGI(TAG, "  MOSI: %d", SPI_MOSI);
  ESP_LOGI(TAG, "  SCLK: %d", SPI_SCLK);
  ESP_LOGI(TAG, "    CS: %d", SPI_LCD_CS);
  ESP_LOGI(TAG, "    DC: %d", SPI_DC);
  ESP_LOGI(TAG, "   RST: %d", SPI_LCD_RESET);
  ESP_LOGI(TAG, "    BL: %d\n", LCD_BL);

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
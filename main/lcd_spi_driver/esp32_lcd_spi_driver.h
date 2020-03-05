/*  Created by Stanislav Lakhtin on 06.01.2020.

    Yao GUAI ESP32 esp-idf SPI driver for Yao-GUAI GFX library

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.
*/

#ifndef YAO_GUAI_OS_TASKS_H
#define YAO_GUAI_OS_TASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "peripherials.h"
#include "lcd/lcd_driver.h"

#ifdef __cplusplus
extern "C"
{
#endif

esp_err_t spi_master_lcd_init(void );
esp_err_t spi_write_lcd_cmnd(uint8_t cmnd );
esp_err_t spi_write_lcd_data(const uint8_t * data, size_t data_length );
esp_err_t spi_read_lcd_data(uint8_t * data, size_t data_length );
esp_err_t display_set_backlight(bool val);

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_OS_TASKS_H

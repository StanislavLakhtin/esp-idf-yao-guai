//
// Created by Stanislav Lakhtin on 05/03/2020.
//

#ifndef ESP32_YAO_GUAI_WEATHER_FIRMWARE_PERIPHERIALS_H
#define ESP32_YAO_GUAI_WEATHER_FIRMWARE_PERIPHERIALS_H

#include "btns/yao_guai_btns.h"

#ifdef CONFIG_IDF_TARGET_ESP32
#define CHIP_NAME "ESP32"
#endif

// LCD Pins

#ifndef LCD_TRANSACTION_COUNT
#define LCD_TRANSACTION_COUNT 6
#endif

#ifndef LCD_DMA_CHAN
#define LCD_DMA_CHAN 2
#endif

#ifndef SPI_CS
#define SPI_LCD_CS 5
#endif

#ifndef SPI_DC
#define SPI_DC 16
#endif

#ifndef SPI_LCD_RESET
#define SPI_LCD_RESET 21
#endif

#ifndef LCD_BL
#define LCD_BL 4
#endif

// Common SPI Master pins

#ifndef SPI_MASTER_0
#define SPI_MASTER_0
#ifndef SPI_MOSI
#define SPI_MOSI 23
#endif

#ifndef SPI_MISO
#define SPI_MISO -1
#endif

#ifndef SPI_SCLK
#define SPI_SCLK 18
#endif
#endif

// SDCard pins

#ifndef SPI_SDCARD_CS
#define SPI_SDCARD_CS 5
#endif

// BTNS pins

#define GPIO_INPUT_ENCODER_0    33
#define GPIO_INPUT_ENCODER_1    35
#define GPIO_INPUT_ENCODER_BTN  34
#define GPIO_INPUT_PIN_SEL      ((1ULL<<GPIO_INPUT_ENCODER_0) | (1ULL<<GPIO_INPUT_ENCODER_1) | (1ULL<<GPIO_INPUT_ENCODER_BTN))

#define ESP_INTR_FLAG_DEFAULT 0

encoder_t encoder0;

#endif //ESP32_YAO_GUAI_WEATHER_FIRMWARE_PERIPHERIALS_H

/*  Created by Stanislav Lakhtin on 08.01.2020.

    Yao GUAI ESP32 esp-idf SPI driver for Yao-GUAI GFX library

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include "esp32_lcd_spi_driver.h"

static const char *TAG = "SPI driver";

spi_device_handle_t spi;

//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
void display_spi_pre_transfer_callback(spi_transaction_t *t) {
  int dc = (int) t->user;
  gpio_set_level(SPI_DC, dc);
}

void display_spi_post_transfer_callback(spi_transaction_t *t) {
  gpio_set_level(SPI_DC, 1);
}

esp_err_t spi_master_lcd_init(void) {
  ESP_LOGI(TAG, "Initialization of SPI\n");
  //Initialize non-SPI GPIOs
  ESP_ERROR_CHECK_WITH_INTERRUPT(gpio_set_direction(SPI_LCD_CS, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK_WITH_INTERRUPT(gpio_set_direction(SPI_DC, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK_WITH_INTERRUPT(gpio_set_direction(SPI_LCD_RESET, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK_WITH_INTERRUPT(gpio_set_direction(LCD_BL, GPIO_MODE_OUTPUT));
  // Reset routine
  ESP_ERROR_CHECK_WITH_INTERRUPT(gpio_set_level(SPI_LCD_RESET, 0));
  delayMs(150);
  ESP_ERROR_CHECK_WITH_INTERRUPT(gpio_set_level(SPI_LCD_RESET, 1));
  delayMs(120);
  // Initializing SPI
  spi_bus_config_t _cfg = {
      .mosi_io_num = SPI_MOSI,       //< GPIO pin for Master Out Slave In (=spi_d) signal, or -1 if not used.
      .miso_io_num = -1,              //< GPIO pin for Master In Slave Out (=spi_q) signal, or -1 if not used.
      .sclk_io_num = SPI_SCLK,       //< GPIO pin for Spi CLocK signal, or -1 if not used.
      .quadwp_io_num = -1,            //< GPIO pin for WP (Write Protect) signal which is used as D2 in 4-bit communication modes, or -1 if not used.
      .quadhd_io_num = -1,            //< GPIO pin for HD (HolD) signal which is used as D3 in 4-bit communication modes, or -1 if not used.
  };
  spi_device_interface_config_t dev = {
      .clock_speed_hz = SPI_MASTER_FREQ_40M,            // Debug/Release clock
      .mode = 0,                                        // SPI mode
      .spics_io_num = SPI_LCD_CS,                          // CS pin
      .queue_size = LCD_TRANSACTION_COUNT,              // We want to be able to queue a few transactions at a time
      .pre_cb = display_spi_pre_transfer_callback,      // Specify pre-transfer callback to handle D/C line
      .post_cb = display_spi_post_transfer_callback,
      .flags = SPI_DEVICE_3WIRE | SPI_DEVICE_HALFDUPLEX,
  };
  // Initialize the SPI bus
  ESP_ERROR_CHECK_WITH_INTERRUPT(spi_bus_initialize(HSPI_HOST, &_cfg, LCD_DMA_CHAN)); // 0 if not use DMA
  // Attach LCD to SPI
  ESP_ERROR_CHECK_WITH_INTERRUPT(spi_bus_add_device(HSPI_HOST, &dev, &spi));
  return ESP_OK;
}

esp_err_t spi_write_lcd_cmnd(const uint8_t cmnd) {
  spi_transaction_t _tx = {
      .length = 8,
      .user = LCD_COMMAND,
      .tx_buffer = &cmnd,
  };
  ESP_LOGD(TAG, "try to send SPI tx cmnd: %02x", cmnd);
  return spi_device_polling_transmit(spi, &_tx);
}

esp_err_t spi_write_lcd_data(const uint8_t *data, size_t data_length) {
  if (!data_length || data == NULL)
    return ESP_OK;
  spi_transaction_t _tx = {
      .length = 8 * data_length,
      .user = LCD_DATA,
      .tx_buffer = data,
  };
  ESP_LOGD(TAG, "try to send SPI tx data length %d", data_length);
  return spi_device_polling_transmit(spi, &_tx);
}

esp_err_t display_set_backlight(bool val) {
  int _bl = (int) val;
  return gpio_set_level(LCD_BL, _bl);
}

esp_err_t spi_read_lcd_data(uint8_t *data, size_t data_length) {
  if (!data_length)
    return ESP_OK;
  spi_transaction_t _tx;
  memset(&_tx, 0x00, sizeof(spi_transaction_t));
  //_tx.length = 8 * data_length;
  _tx.rxlength = 8 * data_length;
  _tx.tx_buffer = 0x0;
  _tx.rx_buffer = data;
  _tx.user = LCD_DATA;

  spi_device_transmit(spi, &_tx);
  return ESP_OK;
}
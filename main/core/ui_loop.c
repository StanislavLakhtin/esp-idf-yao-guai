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
#include "yao-guai.h"
#include "ui_fsm/ui_fsm.h"

static const state_fptr_t ui_states_fn[] = { ui_error_view,
                                             ui_idle_view,
                                             ui_settings_view };

static const io_event_listener_fptr_t default_handlers[] = { NULL,
                                                             ui0_listener,
                                                             NULL};

lcd_device_t ui_dev = { .width = 320, .height = 240,
    .write_cmnd = &spi_write_lcd_cmnd,
    .write_data = &spi_write_lcd_data,
    .read_data = &spi_read_lcd_data,
    .set_backlight = &display_set_backlight,
};

void ui_task(void * args ) {
  ESP_LOGI( TAG, "Hello world from Yao GUAI [meteo]station.\n" );

  lcd_dev = &ui_dev;

  spi_master_lcd_init();  // take a note: you SHOULD check init sequence for you own display in lcd_driver.c

  ESP_LOGI( TAG, "  MOSI: %d", SPI_MOSI );
  ESP_LOGI( TAG, "  SCLK: %d", SPI_SCLK );
  ESP_LOGI( TAG, "    CS: %d", SPI_LCD_CS );
  ESP_LOGI( TAG, "    DC: %d", SPI_DC );
  ESP_LOGI( TAG, "   RST: %d", SPI_LCD_RESET );
  ESP_LOGI( TAG, "    BL: %d\n", LCD_BL );

  ESP_LOGI( TAG, "Detect LCD type\n" );

  int id = get_id( lcd_dev );
  type_lcd_t lcd_detected_type = LCD_TYPE_ILI9340;

  if ( id ) {
    lcd_detected_type = LCD_TYPE_ST7789;      // none-zero, ST7789 chip detected
    ESP_LOGI( TAG, "ST7789V detected.\n" );
  } else
    ESP_LOGI( TAG, "ILI934x detected.\n" );

#ifdef CONFIG_LCD_TYPE_AUTO
  type_lcd_t lcd_type = lcd_detected_type;
#elif defined( CONFIG_LCD_TYPE_ST7789V )
  ESP_LOGI(TAG, "kconfig: force CONFIG_LCD_TYPE_ST7789V.\n");
  type_lcd_t lcd_type = LCD_TYPE_ST7789;
#elif defined( CONFIG_LCD_TYPE_ILI9341 )
  ESP_LOGI(TAG, "kconfig: force CONFIG_LCD_TYPE_ILI9341.\n");
  type_lcd_t lcd_type = LCD_TYPE_ILI9340;
#endif

  lcd_init( lcd_dev, lcd_type );   // take a note, that you MUST call init only after
  ESP_LOGI( TAG, "Display w:h (%d:%d)", lcd_dev->width, lcd_dev->height );
  ui_states_t cur_state = UI_ENTRY_STATE;
  enum ret_codes_t rc;
  state_fptr_t fn;

  loop {
    fn = ui_states_fn[ cur_state ];
    current_input_handler = default_handlers[ cur_state ];
    rc = fn();
    cur_state = ui_lookup_transitions( cur_state, rc );
  }
}
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

#ifndef YAO_GUAI_LCD_DRIVER_H
#define YAO_GUAI_LCD_DRIVER_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "fonts.h"

#define CONFIG_LCD_TYPE_AUTO 0x01

#ifndef delayMs
#define delayMs(xTimeInMs) vTaskDelay(pdMS_TO_TICKS (xTimeInMs) )
#endif

#define DEBUG_ESP

#ifdef DEBUG_ESP
#ifndef ESP_ERROR_CHECK_WITH_INTERRUPT
#define ESP_ERROR_CHECK_WITH_INTERRUPT(x)  do {                         \
        esp_err_t __err_rc = (x);                                       \
        if (__err_rc != ESP_OK) {                                       \
            _esp_error_check_failed(__err_rc, __FILE__, __LINE__,       \
                                    __ASSERT_FUNC, #x);                 \
            return __err_rc;                                            \
        }                                                               \
    } while (0);
#endif
#else
#define ESP_ERROR_CHECK_WITH_INTERRUPT(x) x
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef swap
#define swap(data_type_t, a, b)   \
  {                               \
    data_type_t t = a;            \
    a = b;                        \
    b = t;                        \
  }
#endif

#define MAX 63
typedef struct {
    uint8_t b:6;
    uint8_t g:6;
    uint8_t r:6;
} color_t;

#define set_color(name, R, G, B) name.r = R; name.g = G; name.b = B;
#define is_colors_equal(color1, color2) (color1->r == color2->r \
                                      && color1->g == color1->g \
                                      && color1->b == color1->b )

#define RGB_COLOR_FORMAT_65K  0x50
#define RGB_COLOR_FORMAT_256K 0x60

#define COLOR_FORMAT_12BIT_PIXEL  0x03
#define COLOR_FORMAT_16BIT_PIXEL  0x05
#define COLOR_FORMAT_18BIT_PIXEL  0x06
#define COLOR_FORMAT_TRUNCATED    0x07

#define LCD_COMMAND ( void * ) 0
#define LCD_DATA    ( void * ) 1

#define SET_COLOR_MODE(RGB_FORMAT, COLOR_FORMAT) ( ( RGB_FORMAT << 4 ) || COLOR_FORMAT )

#define NOP                 0x00
#define SWRESET             0x01      // It will be necessary to wait 5 msec before sending new command following software reset.
#define RDDID               0x04      // Read Display ID
#define SLPOUT              0x11      // Sleep OUT
#define NORON               0x13      // Normal Display Mode ON
#define INVOFF              0x20      // Inversion OFF
#define INVON               0x21      // Inversion ON
#define GAMSET              0x26      // Gamma set (1, 2, 4 or 8)
#define DISPOFF             0x28      // Display OFF -- default after SW Reset
#define DISPON              0x29      // Display ON
#define CASET               0x2a      // Columns set (2 uint16 )
#define PASET               0x2b      // Row set (2 uint16)
#define RAMWR               0x2c      // Memory Write
#define MADCTL              0x36      // Memory Data Access Control
#define COLMOD              0x3a

// ==== Display commands constants ====

#define TFT_DISBR        0x51      // 1 byte data 0x00 -- lowest brightness, 0xff -- highest
#define TFT_PTLAR           0x30
#define TFT_ENTRYM          0xB7

#define TFT_CMD_SLPIN       0x10

#define TFT_CMD_RDMODE      0x0A
#define TFT_CMD_RDMADCTL    0x0B
#define TFT_CMD_RDPIXFMT    0x0C
#define TFT_CMD_RDIMGFMT    0x0D
#define TFT_CMD_RDSELFDIAG  0x0F

#define TFT_CMD_FRMCTR1     0xB1
#define TFT_CMD_FRMCTR2     0xB2
#define TFT_CMD_FRMCTR3     0xB3
#define TFT_CMD_INVCTR      0xB4
#define TFT_CMD_DFUNCTR     0xB6

#define TFT_CMD_PWCTR1      0xC0
#define TFT_CMD_PWCTR2      0xC1
#define TFT_CMD_PWCTR3      0xC2
#define TFT_CMD_PWCTR4      0xC3
#define TFT_CMD_PWCTR5      0xC4
#define TFT_CMD_VMCTR1      0xC5
#define TFT_CMD_VMCTR2      0xC7

#define TFT_CMD_RDID1       0xDA
#define TFT_CMD_RDID2       0xDB
#define TFT_CMD_RDID3       0xDC
#define TFT_CMD_RDID4       0xDD

#define TFT_CMD_GMCTRP1     0xE0
#define TFT_CMD_GMCTRN1     0xE1

#define TFT_CMD_POWERA      0xCB
#define TFT_CMD_POWERB      0xCF
#define TFT_CMD_POWER_SEQ   0xED
#define TFT_CMD_DTCA        0xE8
#define TFT_CMD_DTCB        0xEA
#define TFT_CMD_PRC         0xF7
#define TFT_CMD_3GAMMA_EN   0xF2

#define ST_CMD_VCOMS        0xBB
#define ST_CMD_FRCTRL2      0xC6
#define ST_CMD_PWCTR1       0xD0

#define ST7735_FRMCTR1      0xB1
#define ST7735_FRMCTR2      0xB2
#define ST7735_FRMCTR3      0xB3
#define ST7735_INVCTR       0xB4
#define ST7735_DISSET5      0xB6

#define ST7735_PWCTR1       0xC0
#define ST7735_PWCTR2       0xC1
#define ST7735_PWCTR3       0xC2
#define ST7735_PWCTR4       0xC3
#define ST7735_PWCTR5       0xC4
#define ST7735_VMCTR1       0xC5

#define ST7735_RDID1        0xDA
#define ST7735_RDID2        0xDB
#define ST7735_RDID3        0xDC
#define ST7735_RDID4        0xDD
#define ST7735_RDDST        0x09

#define ST7735_PWCTR6       0xFC
#define ST7735_GMCTRP1      0xE0
#define ST7735_GMCTRN1      0xE1

#define MADCTL_MY           0x80
#define MADCTL_MX           0x40
#define MADCTL_MV           0x20
#define MADCTL_ML           0x10
#define MADCTL_MH           0x04

#define TFT_RAMRD           0x2E
#define TFT_CMD_PIXFMT      0x3A

#define TFT_CMD_DELAY       0x80

#define DATALATCH_RIGHT_TO_LEFT_COLUMN_ORDER  (1<<2)
#define BGR_ORDER                             (1<<3)
#define LINE_REFRESH_BOTTOM_TO_TOP_ORDER      (1<<4)
#define REVERSE_PAGE_COLUMN_ORDER             (1<<5)
#define COLUMN_REFRESH_RIGHT_TO_LEFT_ORDER    (1<<6)
#define PAGE_BOTTOM_TO_TOP_ORDER              (1<<7)

#define ROW_ADDRESS_MODE              (1<<7)
#define COLUMN_ADDRESS_MODE           (1<<6)
#define ROW_COLUMN_EXCHANGE           (1<<5)
#define VERTICAL_REFRESH_ORDER        (1<<4)
#define RGB_ORDER                     (1<<3)
#define HORIZONTAL_REFRESH_ORDER      (1<<2)

typedef esp_err_t write_lcd_cmnd(const uint8_t COMMAND);
typedef esp_err_t write_lcd_data(const uint8_t *data, size_t data_length);
typedef esp_err_t read_lcd_data(uint8_t *data, size_t len);
typedef esp_err_t set_backlight(const bool val);

typedef enum {
  LCD_TYPE_ILI9340 = 1,
  LCD_TYPE_ST7789 = 2,
} type_lcd_t;


typedef struct {
  uint16_t width;                     // Device (hardware) width
  uint16_t height;                    // Device (hardware) height
  write_lcd_cmnd *write_cmnd;
  write_lcd_data *write_data;
  read_lcd_data *read_data;
  set_backlight *set_backlight;
} lcd_device_t;

/*
 The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct.
*/
typedef struct {
  uint8_t cmd;
  uint8_t data[16];
  uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;



#define ESP_READY_FOR_NEXT_VALUE 0x01

typedef enum {
  clockwise_nord,
  clockwise_east,
  clockwise_south,
  clockwise_west,
  counterclockwise_nord,
  counterclockwise_west,
  counterclockwise_south,
  counterclockwise_east,
} round_helper_direction_t;

typedef struct {
  uint16_t x0;
  uint16_t y0;
  uint16_t radius;
  uint16_t current_x;
  uint16_t current_y;
  round_helper_direction_t direction;
} round_helper_t;

#define get_color_buf_depth(pixel_count) (pixel_count * 2)

#ifdef __cplusplus
extern "C"
{
#endif

esp_err_t lcd_init(lcd_device_t *dev, type_lcd_t);
uint32_t get_id(lcd_device_t *dev);
esp_err_t _write_addr(lcd_device_t *dev, uint16_t addr0, uint16_t addr1);
esp_err_t frame_set(lcd_device_t *dev, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
esp_err_t frame_fill(lcd_device_t *dev, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye,
                     uint8_t * color_pattern, uint32_t pattern_length, uint32_t count);
esp_err_t color_list_to_bytes(uint8_t *buf, uint32_t max_buf_len, const color_t * color, uint length);
esp_err_t color_list_to_gradient_bytes(uint8_t *buf, uint32_t max_buf_len, uint length,
                                       const color_t * begin_color, const color_t * end_color) ;

esp_err_t draw_pixel(lcd_device_t *dev, uint16_t x, uint16_t y, color_t * color);
esp_err_t draw_circle(lcd_device_t *dev, int16_t x0, int16_t y0, int16_t r, color_t * color);
esp_err_t draw_fast_v_line(lcd_device_t *dev, uint16_t x, uint16_t y, uint16_t h, color_t * color);
esp_err_t draw_fast_h_line(lcd_device_t *dev, uint16_t x, uint16_t y, uint16_t w, color_t * color);
esp_err_t fill_rect(lcd_device_t *dev, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, color_t * color);
esp_err_t draw_fast_h_gradient_line(lcd_device_t *dev, uint16_t x0, uint16_t y0, uint16_t w,
                                    color_t * begin_color, color_t * end_color);

esp_err_t draw_ascii_char(lcd_device_t *dev, int16_t x, int16_t y, unsigned char c,
                          color_t * text_color, color_t * bg_color, ascii_font_size_t size_x,
                          ascii_font_size_t size_y);
esp_err_t draw_ascii_char_fast(lcd_device_t *dev, int16_t x, int16_t y, unsigned char c,
                               color_t * text_color, color_t * bg_color, ascii_font_size_t size_x,
                               ascii_font_size_t size_y);

esp_err_t draw_monospace_text(lcd_device_t *dev, ascii_text_frame_t *text_frame, const char * text, size_t max_length,
                              color_t * color, color_t * bg_color, ascii_font_size_t size_x, ascii_font_size_t size_y);

uint8_t reverse(uint8_t b);

esp_err_t round_helper(round_helper_t * helper);

#ifdef __cplusplus
}
#endif

#endif //YAO_GUAI_LCD_DRIVER_H

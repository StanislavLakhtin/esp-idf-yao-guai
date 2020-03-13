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

#include "lcd.h"

static const char *TAG;
static const char *ST_TAG = "ST7789V";
static const char *ILI_TAG = "ILI934x";

static const lcd_init_cmd_t ili9341_init_cmds[] = {
    /* Power contorl B, power control = 0, DC_ENA = 1 */
    {.cmd = TFT_CMD_POWERB,    {0x00, 0x83, 0X30},                                 3},
    /* Power on sequence control,
     * cp1 keeps 1 frame, 1st frame enable
     * vcl = 0, ddvdh=3, vgh=1, vgl=2
     * DDVDH_ENH=1
     */
    {.cmd = TFT_CMD_POWER_SEQ, {0x64, 0x03, 0X12, 0X81},                           4},
    /* Driver timing control A,
     * non-overlap=default +1
     * EQ=default - 1, CR=default
     * pre-charge=default - 1
     */
    {.cmd = TFT_CMD_DTCA,      {0x85, 0x01, 0x79},                                 3},
    /* Power control A, Vcore=1.6V, DDVDH=5.6V */
    {.cmd = TFT_CMD_POWERA,    {0x39, 0x2C, 0x00, 0x34, 0x02},                     5},
    /* Pump ratio control, DDVDH=2xVCl */
    {.cmd = TFT_CMD_PRC,       {0x20},                                             1},
    /* Driver timing control, all=0 unit */
    {.cmd = TFT_CMD_DTCB,      {0x00, 0x00},                                       2},
    /* Power control 1, GVDD=4.75V */
    {.cmd = TFT_CMD_PWCTR1,    {0x26},                                             1},
    /* Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3 */
    {.cmd = TFT_CMD_PWCTR2,    {0x11},                                             1},
    /* VCOM control 1, VCOMH=4.025V, VCOML=-0.950V */
    {.cmd = TFT_CMD_VMCTR1,    {0x35, 0x3E},                                       2},
    /* VCOM control 2, VCOMH=VMH-2, VCOML=VML-2 */
    {.cmd = TFT_CMD_VMCTR2,    {0xBE},                                             1},
    /* Memory access contorl,*/
    {.cmd = MADCTL,            {RGB_ORDER | ROW_COLUMN_EXCHANGE},   1},
    /* Pixel format, 16bits/pixel for RGB/MCU interface */
    {.cmd = COLMOD,            {RGB_COLOR_FORMAT_256K | COLOR_FORMAT_16BIT_PIXEL}, 1},
    {.cmd = GAMSET,            {0x08}, 1},
    /* Frame rate control, f=fosc, 70Hz fps */
    {.cmd = TFT_CMD_FRMCTR1,   {0x00, 0x1B},                                       2},
    /* Entry mode set, Low vol detect disabled, normal display */
    {.cmd = TFT_ENTRYM,        {0x07},                                             1},
    /* Sleep out */
    {.cmd = SLPOUT,            {0},                                                0x80},
    /* Display on */
    {.cmd = DISPON,            {0},                                                0x80},
    {.cmd = NOP,               {0},                                                0xff},
};

static const lcd_init_cmd_t st7789_init_cmds[] = {
    /* Memory Data Access Control, MX=MV=1, MY=ML=MH=0, RGB=0 */
    {.cmd = MADCTL,         {(1 << 5) | (1 << 6)},          1},
    /* Interface Pixel Format, 16bits/pixel for RGB/MCU interface */
    {.cmd = COLMOD,         {0x55},                         1},
    /* Porch Setting */
    {.cmd = ST7735_FRMCTR2, {0x0c, 0x0c, 0x00, 0x33, 0x33}, 5},
    /* Gate Control, Vgh=13.65V, Vgl=-10.43V */
    {.cmd = TFT_ENTRYM,     {0x45},                         1},
    /* VCOM Setting, VCOM=1.175V */
    {.cmd = ST_CMD_VCOMS,   {0x2B},                         1},
    /* LCM Control, XOR: BGR, MX, MH */
    {.cmd = ST7735_PWCTR1,  {0x2C},                         1},
    /* VDV and VRH Command Enable, enable=1 */
    {.cmd = ST7735_PWCTR2,  {0x01, 0xff},                   2},
    /* VRH Set, Vap=4.4+... */
    {.cmd = ST7735_PWCTR3,  {0x11},                         1},
    /* VDV Set, VDV=0 */
    {.cmd = ST7735_PWCTR4,  {0x20},                         1},
    /* Frame Rate Control, 60Hz, inversion=0 */
    {.cmd = ST_CMD_FRCTRL2, {0x0f},                         1},
    /* Power Control 1, AVDD=6.8V, AVCL=-4.8V, VDDS=2.3V */
    {.cmd = ST_CMD_PWCTR1,  {0xA4, 0xA1},                   1},
    /* Positive Voltage Gamma Control */
    {.cmd = ST7735_GMCTRP1, {0xD0, 0x00, 0x05, 0x0E, 0x15,
                                0x0D, 0x37, 0x43, 0x47, 0x09,
                                0x15, 0x12, 0x16, 0x19},    14},
    /* Negative Voltage Gamma Control */
    {.cmd = ST7735_GMCTRN1, {0xD0, 0x00, 0x05, 0x0D, 0x0C,
                                0x06, 0x2D, 0x44, 0x40, 0x0E,
                                0x1C, 0x18, 0x16, 0x19},    14},
    /* Sleep Out */
    {.cmd = SLPOUT,         {0},                            0x80},
    /* Display On */
    {.cmd = DISPON,         {0},                            0x80},
    {.cmd = NOP,            {0},                            0xff}
};

esp_err_t lcd_init(lcd_device_t *dev, type_lcd_t type) {
  dev->write_cmnd(SWRESET);
  delayMs(120);  // should be at least 5 from Reference Manual from Sitronix ST7789V
  uint16_t cmd_indx = 0;
  const lcd_init_cmd_t *init_cmds;
  if (type == LCD_TYPE_ST7789) {
    init_cmds = st7789_init_cmds;
    TAG = ST_TAG;
  } else {
    init_cmds = ili9341_init_cmds;
    TAG = ILI_TAG;
  }
  while (init_cmds[cmd_indx].databytes != 0xff) {
    dev->write_cmnd(init_cmds[cmd_indx].cmd);
    if (init_cmds[cmd_indx].databytes)
      dev->write_data(init_cmds[cmd_indx].data, init_cmds[cmd_indx].databytes);
    cmd_indx++;
  }
  dev->set_backlight(true);
  ESP_LOGI(TAG, "LCD init complete");
  return ESP_OK;
}

uint32_t get_id(lcd_device_t *dev) {
  uint8_t r[4];
  memset(r, 0x00, 4);
  dev->write_cmnd(RDDID);
  dev->read_data(r, 4);
  uint32_t id = 0x00 | (r[1] << 8) | (r[2] << 16) | (r[3] << 24);
  ESP_LOGD(TAG, "Device ID: %d", id);
  return id;
}

esp_err_t color_list_to_bytes(uint8_t *buf, uint32_t max_buf_len, const color_t * color, uint length) {
  assert (max_buf_len >= (length / 2));
  uint32_t _ic = 0x00;
  for (int i = 0; i < length; i++) {
    buf[_ic] = (color->r << 3) | (color->g >> 3); _ic++;
    buf[_ic] = (color->g << 2 & 0b11100000) | color->b; _ic++;
  }
  return ESP_OK;
}

//todo unfinished. add color 16/12 depth and calc right way
esp_err_t color_list_to_gradient_bytes(uint8_t *buf, uint32_t max_buf_len, uint length,
                                       const color_t * begin_color, const color_t * end_color) {
  uint _cic = get_color_buf_depth(length);
  assert (max_buf_len >= _cic);
  int16_t delta_R = end_color->r - begin_color->r;
  int16_t delta_G = end_color->g - begin_color->g;
  int16_t delta_B = end_color->b - begin_color->b;
  color_t current_color;
  for (int i = 0; i < length; i++) {
    current_color.r = begin_color->r + i * delta_R / length;
    current_color.g = begin_color->g + i * delta_G / length;
    current_color.b = begin_color->b + i * delta_B / length;
    color_list_to_bytes(buf + (i << 1), 2, &current_color, 1);
  }
  return ESP_OK;
}

esp_err_t _write_addr(lcd_device_t *dev, uint16_t addr0, uint16_t addr1) {
  uint8_t buffer[4];
  buffer[0] = (addr0 >> 8) & 0xFF;
  buffer[1] = addr0 & 0xFF;
  buffer[2] = (addr1 >> 8) & 0xFF;
  buffer[3] = addr1 & 0xFF;
  return dev->write_data(buffer, sizeof(buffer));
}

esp_err_t frame_set(lcd_device_t *dev, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye) {
  if (xe < xs)
    swap(uint16_t, xs, xe);
  if (ye < ys)
    swap(uint16_t, ye, ys);
  ESP_ERROR_CHECK_WITH_INTERRUPT(dev->write_cmnd(CASET));
  ESP_ERROR_CHECK_WITH_INTERRUPT(_write_addr(dev, xs, xe));
  ESP_ERROR_CHECK_WITH_INTERRUPT(dev->write_cmnd(PASET));
  ESP_ERROR_CHECK_WITH_INTERRUPT(_write_addr(dev, ys, ye));
  return ESP_OK;
}

/// Filling a frame with a pre-prepared color array.

esp_err_t frame_fill(lcd_device_t *dev, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye,
    uint8_t * color_pattern, uint32_t pattern_length, uint32_t count) {
  ESP_ERROR_CHECK_WITH_INTERRUPT(frame_set(dev, xs, xe, ys, ye));
  ESP_ERROR_CHECK_WITH_INTERRUPT(dev->write_cmnd(RAMWR));
  for (int i = 0; i < count; i++) {
    ESP_ERROR_CHECK_WITH_INTERRUPT(dev->write_data(color_pattern, pattern_length));
  }
  return ESP_OK;
}

//todo make direction, check it
esp_err_t round_helper(round_helper_t *helper) {
  int16_t f = 1 - helper->radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * helper->radius;
  int16_t x = helper->current_x;
  int16_t y = helper->current_y;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    return ESP_READY_FOR_NEXT_VALUE;
  }
  return ESP_OK;
}

uint8_t reverse(uint8_t b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}


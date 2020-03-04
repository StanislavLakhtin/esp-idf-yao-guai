//
// Created by Stanislav Lakhtin on 06/02/2020.
//

#include "yao-guai.h"
#include <esp_vfs_fat.h>
#include "driver/sdspi_host.h"

#define WAIT_TRESHOLD 1000

static const char* TAG = "sdcard";
static const sdmmc_host_t host = SDSPI_HOST_DEFAULT();
static sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
static esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
};

#define MOUNT_POINT  "/sdcard"
#define SYS_FOLDER  "/.system"
#define SYS_EXT ".cbor"
#define CONNECTION  "/wifi"
#define CONNECTION_FILE_PATH MOUNT_POINT SYS_FOLDER CONNECTION SYS_EXT

#define SSID_LIST_NAME      "wifi_aps"
#define SSID_PASSWORD_NAME  "password"
#define SSID_PRIORITY_NAME  "priority"
#define SSID_PRIORITY_DEFINE_VALUE  10

void conf_init(conf_t * conf) {
  conf->inUse = xSemaphoreCreateMutex();
  slot_config.gpio_miso = PIN_NUM_MISO;
  slot_config.gpio_mosi = PIN_NUM_MOSI;
  slot_config.gpio_sck  = PIN_NUM_CLK;
  slot_config.gpio_cs   = PIN_NUM_CS;
}

esp_err_t sdcard_session_start(conf_t* conf) {
  if (xSemaphoreTake(conf->inUse, (TickType_t) WAIT_TRESHOLD) != pdTRUE) {
    return ESP_ERR_INVALID_STATE;
  }
  ESP_LOGI(TAG, "Initializing SD card (SPI)");
  esp_err_t ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &conf->sd_card);
  sdmmc_card_print_info(stdout, conf->sd_card);
  return ret;
}

esp_err_t sdcard_session_finish(conf_t* conf) {
  xSemaphoreGive( conf->inUse );
  return ESP_OK;
}

static esp_err_t make_sysfolders_if_necessary() {
  //todo make all necessary folders on SDCARD
  return ESP_OK;
}

esp_err_t is_ssid_conf_exists(conf_t *conf, const char * ssid) {
  if (xSemaphoreTake(conf->inUse, (TickType_t) WAIT_TRESHOLD) != pdTRUE) {
    return ESP_ERR_INVALID_STATE;
  }
  esp_err_t err = sdcard_session_start(conf);
  if ( !err ) {
    // todo
    FILE* fp = fopen(CONNECTION_FILE_PATH, "r");
    if (fp == NULL)
      return ESP_ERR_NOT_FOUND;
    filestat _st;
    if (fstat(fileno(fp), &_st)) {
      ESP_LOGE(TAG, "Can't read attribute of file %s", CONNECTION_FILE_PATH);
      fclose(fp);
      return ESP_ERR_NOT_SUPPORTED;
    }
    uint8_t * buffer = malloc(_st.st_size);
    if (buffer == NULL) {
      ESP_LOGE(TAG, "Can't allocate %lu bytes to read file %s", _st.st_size, CONNECTION_FILE_PATH);
      fclose(fp);
      return ESP_ERR_NOT_SUPPORTED;
    }
    size_t size = fread(buffer, _st.st_size, 1, fp);
    fclose(fp);
    if (err) {
      free(buffer);
    }

    free(buffer);
  } else {
    ESP_LOGE(TAG, "SD-Card error: %d", err);
  }
  xSemaphoreGive( conf->inUse );
  return ESP_OK;
}

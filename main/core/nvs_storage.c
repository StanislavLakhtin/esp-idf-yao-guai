//
// Created by sl on 05.04.2020.
//

#include "nvs_storage.h"

static const char * TAG = "NVS";

esp_err_t load_conf(stored_ssid_conf_t * conf, uint8_t * count) {
  nvs_handle_t nvs_handle;
  esp_err_t err;
  size_t sz;

  // Open
  err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) return err;

  memset(conf_version, 0x00, 33);
  err = nvs_get_str(nvs_handle, SSIDS_VERSION_STORAGE_NAME, conf_version, &sz);
  if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)  {
    ESP_LOGE(TAG, "Error: %d", err);
    return err;
  }
  if (err == ESP_ERR_NVS_NOT_FOUND)  {
    ESP_LOGW(TAG, "Conf not found");
  } else
    ESP_LOGI(TAG, "Conf version: %s", conf_version);

  if (!strcmp(conf_version, YG_STORAGE_VERSION)) {
    ESP_LOGE(TAG, "Incompatible conf version. Should be %s. Instead was found %s", YG_STORAGE_VERSION, conf_version);
    nvs_close(nvs_handle);
    return ESP_OK;
  }

  *count = 0;
  err = nvs_get_u8(nvs_handle, SSIDS_LIST_COUNT_STORAGE_NAME, count);
  if (err == ESP_ERR_NVS_NOT_FOUND)  {
    ESP_LOGW(TAG, "Parameter %s not found", SSIDS_LIST_COUNT_STORAGE_NAME);
  } else
    ESP_LOGI(TAG, "Totaly stored %d ap ssid's", *count);

  if (! *count) {
    nvs_close(nvs_handle);
    return ESP_OK;
  }

  if (conf != NULL) {
    ESP_LOGI(TAG, "Free conf mem");
    free(conf);
  }

  uint32_t required_size = sizeof(stored_ssid_conf_t) * *count + sizeof(uint32_t);
  conf = malloc( required_size );
  memset(conf, 0x00, required_size);
  err = nvs_get_blob(nvs_handle, SSIDS_LIST_STORAGE_NAME, conf, &required_size);
  if (err != ESP_OK) {
    free(conf);
  }

  nvs_close(nvs_handle);
  return err;
}

esp_err_t store_conf() {
  nvs_handle_t nvs_handle;
  esp_err_t err;

  // Open
  err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) return err;

  err = nvs_set_str(nvs_handle, YG_STORAGE_VERSION, conf_version);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Store esp error: %d", err);
    nvs_close(nvs_handle);
    return err;
  }
  err = nvs_set_u8(nvs_handle, SSIDS_LIST_COUNT_STORAGE_NAME, stored_aps_count);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Store ESP NVS error: %d", err);
    nvs_close(nvs_handle);
    return err;
  }
  uint32_t required_size = sizeof(stored_ssid_conf_t) * stored_aps_count + sizeof(uint32_t);
  err = nvs_set_blob(nvs_handle, SSIDS_LIST_STORAGE_NAME, stored_aps, required_size);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Store ESP NVS error: %d", err);
    nvs_close(nvs_handle);
    return err;
  }

  nvs_close(nvs_handle);
  return ESP_OK;
}

esp_err_t store_ap_to_conf(stored_ssid_conf_t * ap_ssid) {
  esp_err_t err;
  stored_ssid_conf_t * _ap;

  for (int i=0; i < stored_aps_count; i++) {
    _ap = &stored_aps[i];
    if (strcmp(_ap->ssid, ap_ssid->ssid)) {
      strncpy(_ap->ssid, ap_ssid->ssid, ESP_SSID_MAX_LENGTH);
      strncpy(_ap->password, ap_ssid->password, ESP_SSID_MAX_LENGTH);
      _ap->priority = ap_ssid->priority;
      err = store_conf();
      return err;
    }
  }

  // Ap not found. Increase stored_aps_count to 1 to add new ap on the next step
  stored_aps_count += 1;
  err = load_conf(stored_aps, &stored_aps_count);
  if (err != ESP_OK) {
    return err;
  }

  _ap = &stored_aps[stored_aps_count - 1];
  strncpy(_ap->ssid, ap_ssid->ssid, ESP_SSID_MAX_LENGTH);
  strncpy(_ap->password, ap_ssid->password, ESP_SSID_MAX_LENGTH);
  _ap->priority = ap_ssid->priority;

  err = store_conf();
  return err;
}

idf_component_register( SRC_DIRS "ow"
                                 "ow_uart_driver"
                                 "core"
                                 "core/wifi_fsm"
                                 "encoder"
                                 "screens"
                                 "."
                        INCLUDE_DIRS "."
                                     "screens"
                                     "core"
                                     "encoder"
                        REQUIRES nvs_flash lvgl lvgl_esp32_drivers lvgl_tft
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE LV_CONF_INCLUDE_SIMPLE=1)

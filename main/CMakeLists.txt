idf_component_register( SRC_DIRS "./ow"
                                 "./ow_uart_driver"
                                 "./core"
                                 "./core/wifi_fsm"
                                 "."
                        INCLUDE_DIRS "."
                        REQUIRES lvgl lvgl_tft lvgl_esp32_drivers lvgl_touch lvgl_examples
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE LV_CONF_INCLUDE_SIMPLE=1)

idf_component_register( SRC_DIRS "./ow"
                                 "./ow_uart_driver"
                                 "./core"
                                 "./core/wifi_fsm"
                                 "."
                        INCLUDE_DIRS "."
                        REQUIRES lvgl_esp32_drivers lvgl lvgl_examples lvgl_tft lvgl_touch
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE LV_CONF_INCLUDE_SIMPLE=1)

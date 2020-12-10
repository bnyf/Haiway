/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "foo.h"

#include "jerryscript.h"
#include "jerryscript-ext/handler.h"
#include "jerryscript-port.h"


#define CHIP_NAME "ESP32"

void jerry_ext_handler_init() {
  jerryx_handler_register_global ((const jerry_char_t *) "print",
                                  jerryx_handler_print);
}

void app_main(void)
{
    printf("Hello world!\n");
    printf("foo: %d\n",foo());
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
            CHIP_NAME,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    /* Initialize engine */
    jerry_init(JERRY_INIT_EMPTY);
    jerry_ext_handler_init();
    jerry_char_t script[100] = "print ('haiway')";

    for (int i = 10; i >= 0; i--) {
        jerry_value_t parsed_code = jerry_parse (NULL, 0, script, strlen((char*)script), JERRY_PARSE_NO_OPTS);
        if (jerry_value_is_error(parsed_code)) {
          printf("Unexpected error\n");
        } else {
          jerry_value_t ret_value = jerry_run(parsed_code);
          jerry_release_value(ret_value);
        }
        jerry_release_value(parsed_code);
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
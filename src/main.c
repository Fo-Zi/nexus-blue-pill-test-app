/**
 * @file main.c
 * @brief Basic Blue Pill test application using nexus-llhal-stm32f103
 *
 * This application demonstrates basic GPIO functionality using the
 * Nexus Embedded Ecosystem with the STM32F103 HAL implementation.
 */

#include <stdint.h>
#include <stdbool.h>

#include "platform_config.h"
#include "nhal_pin.h"
#include "nhal_common.h"
#include "stm32f103_helpers.h"

/**
 * @brief Simple LED blink test
 *
 * Blinks the built-in LED with 1 second delay between off-on cycles
 */

static void led_blink_test(void) {
    struct nhal_pin_context *led_ctx = platform_get_pin_ctx(PIN_LED_BUILTIN);

    if (!led_ctx) {
        stm32f103_uart_debug_print("ERROR: LED context is NULL!\r\n");
        return;
    }

    stm32f103_uart_debug_print("Starting LED blink test...\r\n");

    while (1) {
        // Turn LED on (try both polarities)
        stm32f103_uart_debug_print("LED HIGH\r\n");
        nhal_pin_set_state(led_ctx, NHAL_PIN_HIGH);
        nhal_delay_milliseconds(1000);

        // Turn LED off
        stm32f103_uart_debug_print("LED LOW\r\n");
        nhal_pin_set_state(led_ctx, NHAL_PIN_LOW);
        nhal_delay_milliseconds(1000);
    }
}

/**
 * @brief Main application entry point
 */
int main(void) {
    // Initialize platform hardware
    nhal_result_t result = platform_init();
    if (result != NHAL_OK) {
        // Platform initialization failed - halt with error message
        stm32f103_uart_debug_print("FATAL: Platform init failed!\r\n");
        while (1);
    }

    // Print startup information
    stm32f103_uart_debug_print("\r\n=== Blue Pill Test App Started ===\r\n");
    stm32f103_uart_debug_print_num("System Clock: ", stm32f103_get_sysclk_hz());
    stm32f103_uart_debug_print("Platform init: SUCCESS\r\n");

    // Run LED blink test (infinite loop)
    led_blink_test();

    // Should never reach here
    return 0;
}

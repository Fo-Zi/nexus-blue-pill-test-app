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

/**
 * @brief Simple LED blink test
 * 
 * Blinks the built-in LED with 1 second delay between off-on cycles
 */
// Forward declare debug functions
extern void stm32f103_uart_debug_print(const char* str);

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
 * @brief GPIO input/output test
 * 
 * Tests digital output on debug pin and reads test input
 */
static void gpio_io_test(void) {
    struct nhal_pin_context *debug_ctx = platform_get_pin_ctx(PIN_DEBUG_OUTPUT);
    struct nhal_pin_context *input_ctx = platform_get_pin_ctx(PIN_TEST_INPUT);
    
    if (!debug_ctx || !input_ctx) {
        return;
    }
    
    for (int i = 0; i < 10; i++) {
        // Set debug output high
        nhal_pin_set_state(debug_ctx, NHAL_PIN_HIGH);
        nhal_delay_milliseconds(100);
        
        // Read test input
        nhal_pin_state_t input_state;
        nhal_pin_get_state(input_ctx, &input_state);
        
        // Set debug output low
        nhal_pin_set_state(debug_ctx, NHAL_PIN_LOW);
        nhal_delay_milliseconds(100);
    }
}

/**
 * @brief Main application entry point
 */
int main(void) {
    // Initialize platform hardware
    nhal_result_t result = platform_init();
    if (result != NHAL_OK) {
        // Platform initialization failed - halt
        while (1);
    }
    
    // Run LED blink test (infinite loop)
    led_blink_test();
    
    // Should never reach here
    return 0;
}
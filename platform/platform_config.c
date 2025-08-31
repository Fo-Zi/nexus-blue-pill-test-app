/**
 * @file platform_config.c
 * @brief Platform Integration implementation for Blue Pill (STM32F103C8T6)
 *
 * Maps logical pin functions to actual STM32F103 hardware pins and
 * manages context allocation and initialization.
 */

#include "platform_config.h"
#include "nhal_pin_stm32f103.h"
#include "stm32f103_helpers.h"
#include <stddef.h>

// Memory allocation for NHAL contexts and configs
static struct nhal_pin_context pin_contexts[PIN_TOTAL_NUM];
static struct nhal_pin_config pin_configs[PIN_TOTAL_NUM];

// STM32F103-specific implementation configs (no impl_context needed per the header)
static struct nhal_pin_impl_config pin_impl_configs[PIN_TOTAL_NUM];

// STM32F103 pin IDs for hardware mapping
static struct nhal_pin_id pin_ids[PIN_TOTAL_NUM];

// Platform configuration initialization
static void setup_pin_mappings(void) {
    // PIN_LED_BUILTIN -> PC13 (Blue Pill built-in LED)
    pin_ids[PIN_LED_BUILTIN] = (struct nhal_pin_id){
        .port = STM32F103_GPIO_PORT_C,
        .pin = STM32F103_GPIO_PIN_13
    };
    
    pin_configs[PIN_LED_BUILTIN] = (struct nhal_pin_config){
        .direction = NHAL_PIN_DIR_OUTPUT,
        .pull_mode = NHAL_PIN_PMODE_NONE,
        .impl_config = &pin_impl_configs[PIN_LED_BUILTIN]
    };
    
    pin_impl_configs[PIN_LED_BUILTIN] = (struct nhal_pin_impl_config){
        .reserved = 0
    };
    
    // PIN_USER_BUTTON -> PA0 (example user button)
    pin_ids[PIN_USER_BUTTON] = (struct nhal_pin_id){
        .port = STM32F103_GPIO_PORT_A,
        .pin = STM32F103_GPIO_PIN_0
    };
    
    pin_configs[PIN_USER_BUTTON] = (struct nhal_pin_config){
        .direction = NHAL_PIN_DIR_INPUT,
        .pull_mode = NHAL_PIN_PMODE_PULL_UP,
        .impl_config = &pin_impl_configs[PIN_USER_BUTTON]
    };
    
    pin_impl_configs[PIN_USER_BUTTON] = (struct nhal_pin_impl_config){
        .reserved = 0
    };
    
    // PIN_DEBUG_OUTPUT -> PA1 (general purpose output)
    pin_ids[PIN_DEBUG_OUTPUT] = (struct nhal_pin_id){
        .port = STM32F103_GPIO_PORT_A,
        .pin = STM32F103_GPIO_PIN_1
    };
    
    pin_configs[PIN_DEBUG_OUTPUT] = (struct nhal_pin_config){
        .direction = NHAL_PIN_DIR_OUTPUT,
        .pull_mode = NHAL_PIN_PMODE_NONE,
        .impl_config = &pin_impl_configs[PIN_DEBUG_OUTPUT]
    };
    
    pin_impl_configs[PIN_DEBUG_OUTPUT] = (struct nhal_pin_impl_config){
        .reserved = 0
    };
    
    // PIN_TEST_INPUT -> PA2 (test input with pull-up)
    pin_ids[PIN_TEST_INPUT] = (struct nhal_pin_id){
        .port = STM32F103_GPIO_PORT_A,
        .pin = STM32F103_GPIO_PIN_2
    };
    
    pin_configs[PIN_TEST_INPUT] = (struct nhal_pin_config){
        .direction = NHAL_PIN_DIR_INPUT,
        .pull_mode = NHAL_PIN_PMODE_PULL_UP,
        .impl_config = &pin_impl_configs[PIN_TEST_INPUT]
    };
    
    pin_impl_configs[PIN_TEST_INPUT] = (struct nhal_pin_impl_config){
        .reserved = 0
    };
}

static void setup_pin_contexts(void) {
    for (int i = 0; i < PIN_TOTAL_NUM; i++) {
        pin_contexts[i] = (struct nhal_pin_context){
            .pin_id = &pin_ids[i],
            .impl_ctx = NULL  // No impl_ctx needed per STM32F103 implementation
        };
    }
}

nhal_result_t platform_init(void) {
    // Blue Pill (STM32F103C8T6) platform-specific configuration
    stm32f103_system_config_t system_config = {
        .target_sysclk_hz = 72000000,  // 72MHz target (typical for Blue Pill)
        .enable_systick = true,         // Enable SysTick for timing
        .systick_freq_hz = 1000         // 1kHz SysTick (1ms ticks)
    };
    
    // Initialize STM32F103 system with platform configuration
    if (stm32f103_system_init(&system_config) != 0) {
        return NHAL_ERR_HW_FAILURE;
    }
    
    // Initialize UART debugging
    stm32f103_uart_debug_init();
    stm32f103_uart_debug_print("Platform init starting...\r\n");
    stm32f103_uart_debug_print_num("System clock: ", stm32f103_get_sysclk_hz());
    
    // Setup hardware mappings and configurations
    setup_pin_mappings();
    setup_pin_contexts();
    
    // Initialize all pins
    for (int i = 0; i < PIN_TOTAL_NUM; i++) {
        stm32f103_uart_debug_print_num("Initializing pin: ", i);
        nhal_result_t result = nhal_pin_init(&pin_contexts[i]);
        if (result != NHAL_OK) {
            stm32f103_uart_debug_print_num("Pin init failed: ", result);
            return result;
        }
        
        result = nhal_pin_set_config(&pin_contexts[i], &pin_configs[i]);
        if (result != NHAL_OK) {
            stm32f103_uart_debug_print_num("Pin config failed: ", result);
            return result;
        }
        stm32f103_uart_debug_print("Pin OK\r\n");
    }
    
    stm32f103_uart_debug_print("Platform init complete!\r\n");
    return NHAL_OK;
}

struct nhal_pin_context* platform_get_pin_ctx(pin_func_t pin_func) {
    if (pin_func >= PIN_TOTAL_NUM) {
        return NULL;
    }
    
    return &pin_contexts[pin_func];
}
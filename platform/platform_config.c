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
            .callback = NULL,
            .user_data = NULL,
            .trigger = NHAL_PIN_INT_TRIGGER_NONE,
            .interrupt_configured = false,
            .interrupt_enabled = false
        };
    }
}

nhal_result_t platform_init(void) {
    // Get default 72MHz configuration and initialize clock system
    stm32f103_clock_config_t clock_config;
    stm32f103_get_default_72mhz_config(&clock_config);
    
    // Initialize clock - function handles all recovery automatically
    if (stm32f103_clock_init(&clock_config, NULL) != 0) {
        return NHAL_ERR_HW_FAILURE;
    }

    // Initialize debug UART for main.c printing (non-critical)
    stm32f103_uart_debug_init();

    // Setup hardware mappings and configurations
    setup_pin_mappings();
    setup_pin_contexts();

    // Initialize all pins
    for (int i = 0; i < PIN_TOTAL_NUM; i++) {
        nhal_result_t result = nhal_pin_init(&pin_contexts[i]);
        if (result != NHAL_OK) {
            return result;
        }

        result = nhal_pin_set_config(&pin_contexts[i], &pin_configs[i]);
        if (result != NHAL_OK) {
            return result;
        }
    }

    return NHAL_OK;
}

struct nhal_pin_context* platform_get_pin_ctx(pin_func_t pin_func) {
    if (pin_func >= PIN_TOTAL_NUM) {
        return NULL;
    }

    return &pin_contexts[pin_func];
}

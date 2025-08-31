/**
 * @file platform_config.h
 * @brief Platform Integration Layer for Blue Pill (STM32F103C8T6)
 *
 * This file defines logical resources for the Blue Pill board and provides
 * the interface between application code and the STM32F103 HAL implementation.
 */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#include "nhal_pin.h"
#include "nhal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Logical pin definitions for Blue Pill board
 * 
 * These abstract pin functions can be mapped to actual STM32F103 pins
 * in the platform implementation layer.
 */
typedef enum {
    PIN_LED_BUILTIN,        /**< Built-in LED (typically PC13) */
    PIN_USER_BUTTON,        /**< User button for testing */
    PIN_DEBUG_OUTPUT,       /**< General purpose debug output pin */
    PIN_TEST_INPUT,         /**< Test input pin with pull-up */
    PIN_TOTAL_NUM           /**< Total number of logical pins */
} pin_func_t;

/**
 * @brief Initialize all platform hardware resources
 * 
 * This function initializes all GPIO pins and other hardware resources
 * defined in the platform configuration. Must be called before using
 * any hardware peripherals.
 * 
 * @return NHAL_OK on success, error code otherwise
 */
nhal_result_t platform_init(void);

/**
 * @brief Get pin context for a logical pin function
 * 
 * @param pin_func Logical pin function identifier
 * @return Pointer to initialized pin context, NULL if invalid
 */
struct nhal_pin_context* platform_get_pin_ctx(pin_func_t pin_func);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_CONFIG_H */
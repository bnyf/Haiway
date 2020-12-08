/***************************************************************************//**
* \file cyhal_timer.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Timer/Counter.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_hal_timer Timer (Timer/Counter)
* \ingroup group_hal
* \{
* High level interface for interacting with the Timer/Counter hardware resource.
*
* The timer block is commonly used to measure the time of occurrence of an event,
* to measure the time difference between two events or perform an action after
* a specified period of time. The driver also allows the user to invoke a callback function
* when a particular event occurs.
*
* Some use case scenarios of timer -
*
* * Creating a periodic interrupt for executing periodic tasks
* * Measuring time between two events
* * Triggering other system resources after a certain number of events
* * Capturing time stamps when events occur
*
* \section subsection_timer_features Features
* * Runtime configurable parameters like period and compare value - \ref cyhal_timer_cfg_t
* * Configurable counting direction - \ref cyhal_timer_direction_t
* * Interrupt on various events - \ref cyhal_timer_event_t
* * Continuous or One Shot run modes
*
* \section subsection_timer_quickstart Quick Start
*
* \ref cyhal_timer_init can be used for timer initialization by providing the timer object - \ref cyhal_timer_t,
* and shared clock source - <b> clk </b> (optional). The timer parameters needs to be populated in \ref cyhal_timer_cfg_t structure.
* The timer then needs to be configured by using the \ref cyhal_timer_configure function.
*
* \note A default frequency is set when an existing clock divider - <b> clk </b> is not provided to \ref cyhal_timer_init which is
* defined by the macro - \ref CYHAL_TIMER_DEFAULT_FREQ.
*
* \warning Currently there is no support for pin connections to Timer using this driver. So, the <b> pin </b> should be
* assigned as \ref NC while using the \ref cyhal_timer_init to initialize the timer.
*
*
* See \ref subsection_timer_snippet_1.
*
* \section subsection_timer_sample_snippets Code Snippets
*
* \subsection subsection_timer_snippet_1 Snippet 1: Measuring time between two events
* The following snippet initializes a Timer and measures the time between two events.
* The <b>clk</b> need not be provided, in which case a clock resource is assigned.
* \snippet timer.c snippet_cyhal_timer_event_measure
*
* \subsection subsection_timer_snippet_2 Snippet 2: Handling an event in a callback function
* The following snippet initializes a Timer and triggers an event after every one second.
* The <b>clk</b> need not be provided (NULL), in which
* case a clock resource is assigned.
* \snippet timer.c snippet_cyhal_timer_event_interrupt
*
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*       Enumerations
*******************************************************************************/

/** Timer directions */
typedef enum
{
    CYHAL_TIMER_DIR_UP, //!< Counts up
    CYHAL_TIMER_DIR_DOWN, //!< Counts down
    CYHAL_TIMER_DIR_UP_DOWN, //!< Counts up and down, terminal count occurs on both overflow and underflow.
} cyhal_timer_direction_t;

/** Timer/counter interrupt triggers */
typedef enum {
    CYHAL_TIMER_IRQ_NONE            =  0, /**< No interrupt handled **/
    CYHAL_TIMER_IRQ_TERMINAL_COUNT  =  1 << 0, /**< Interrupt when terminal count is reached **/
    CYHAL_TIMER_IRQ_CAPTURE_COMPARE =  1 << 1, /**< Interrupt when Compare/Capture value is reached **/
    CYHAL_TIMER_IRQ_ALL             = (1 << 2) - 1, /**< Interrupt on terminal count and Compare/Capture values **/
} cyhal_timer_event_t;

/*******************************************************************************
*       Data Structures
*******************************************************************************/

/** @brief Describes the current configuration of a timer/counter */
typedef struct
{
    /**
      * Whether the timer is set to continuously run.
      * If true, the timer will run forever.
      * Otherwise, the timer will run once and stop (one shot).
      */
    bool is_continuous; //!< Whether the timer/counter operates continuous (true) or one shot (false)
    cyhal_timer_direction_t direction; //!< Direction the timer/counter is running
    bool is_compare; //!< Is it in compare (true) or capture (false) mode
    uint32_t period; //!< Timer/counter period
    uint32_t compare_value; //!< Timer/counter comparison value
    uint32_t value; //!< Current value of the timer/counter
} cyhal_timer_cfg_t;

/*******************************************************************************
*       Typedefs
*******************************************************************************/

/** Handler for timer events */
typedef void(*cyhal_timer_event_callback_t)(void *callback_arg, cyhal_timer_event_t event);

/** Bad argument. eg: null pointer */
#define CYHAL_TIMER_RSLT_ERR_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TIMER, 0))
/** Failed to initialize Timer clock */
#define CYHAL_TIMER_RSLT_ERR_CLOCK_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TIMER, 1))
/** Failed to initialize Timer */
#define CYHAL_TIMER_RSLT_ERR_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TIMER, 2))
/** Cannot change the timer frequency when a shared clock divider is in use */
#define CYHAL_TIMER_RSLT_ERR_SHARED_CLOCK (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TIMER, 3))

/** Default timer frequency, used when an existing clock divider is not provided to init */
#define CYHAL_TIMER_DEFAULT_FREQ (1000000u)

/*******************************************************************************
*       Functions
*******************************************************************************/

/** Initialize the timer/counter peripheral and configure the pin. <br>
 * See \ref subsection_timer_snippet_1.
 *
 * @param[out] obj The timer/counter object to initialize
 * @param[in]  pin optional - The timer/counter compare/capture pin to initialize
 * @param[in]  clk optional - The shared clock to use, if not provided a new clock will be allocated
 *                  and the timer frequency will be set to CYHAL_TIMER_DEFAULT_FREQ
 * @return The status of the init request
 */
cy_rslt_t cyhal_timer_init(cyhal_timer_t *obj, cyhal_gpio_t pin, const cyhal_clock_divider_t *clk);

/** Deinitialize the timer/counter object
 *
 * @param[in,out] obj The timer/counter object
 */
void cyhal_timer_free(cyhal_timer_t *obj);

/** Updates the configuration of the timer/counter object <br>
 * See \ref subsection_timer_snippet_1.
 * @param[in] obj  The timer/counter object
 * @param[in] cfg  The configuration of the timer/counter
 * @return The status of the configure request
 */
cy_rslt_t cyhal_timer_configure(cyhal_timer_t *obj, const cyhal_timer_cfg_t *cfg);

/** Configures the timer frequency.
 * \note This is only valid to call if a null clock divider was provided to \ref cyhal_timer_init.
 * If a custom clock was provided its frequency should be adjusted directly.
 *
 * See \ref subsection_timer_snippet_1.
 * @param[in] obj  The timer/counter object
 * @param[in] hz   The frequency rate in Hz
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_timer_set_frequency(cyhal_timer_t *obj, uint32_t hz);

/** Starts the timer/counter with the pre-set configuration <br>
 * See \ref subsection_timer_snippet_1.
 *
 * @param[in] obj     The timer/counter object
 * @return The status of the start request
 */
cy_rslt_t cyhal_timer_start(cyhal_timer_t *obj);

/** Stops the timer/counter <br>
 * See \ref subsection_timer_snippet_1.
 *
 * @param[in] obj     The timer/counter object
 * @return The status of the stop request
 */
cy_rslt_t cyhal_timer_stop(cyhal_timer_t *obj);

/** Reads the current value from the timer/counter <br>
 * See \ref subsection_timer_snippet_1.
 *
 * @param[in] obj     The timer/counter object
 * @return The current value of the timer/counter
 */
uint32_t cyhal_timer_read(const cyhal_timer_t *obj);

/** The timer/counter callback handler registration <br>
 * See \ref subsection_timer_snippet_2.
 *
 * @param[in] obj          The timer/counter object
 * @param[in] callback     The callback handler which will be invoked when the event occurs
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_timer_register_callback(cyhal_timer_t *obj, cyhal_timer_event_callback_t callback, void *callback_arg);

/** Configure timer/counter event enablement <br>
 * See \ref subsection_timer_snippet_2.
 *
 * @param[in] obj           The timer/counter object
 * @param[in] event         The timer/counter event type
 * @param[in] intr_priority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_timer_enable_event(cyhal_timer_t *obj, cyhal_timer_event_t event, uint8_t intr_priority, bool enable);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_TIMER_IMPL_HEADER
#include CYHAL_TIMER_IMPL_HEADER
#endif /* CYHAL_TIMER_IMPL_HEADER */

/** \} group_hal_timer */
#ifndef __PINOUT_HEADER_H__
#define __PINOUT_HEADER_H__

// Hardware Revision ID
#include "revisions.h"
#define REVISION_ID                                 REVISION_EVB

// Buttons
#define PIN_BUTTON_1                                AM_BSP_GPIO_BUTTON0
#define PIN_BUTTON_2                                AM_BSP_GPIO_BUTTON1

// LEDs
#define PIN_LED1_RED                                AM_BSP_GPIO_LED0
#define PIN_LED1_GREEN                              AM_BSP_GPIO_LED1
#define PIN_LED2_RED                                AM_BSP_GPIO_LED0
#define PIN_LED2_YELLOW                             AM_BSP_GPIO_LED2

// Logging
#define PIN_SWO                                     28
#define PIN_SWO_FUNCTION                            AM_HAL_PIN_28_SWO

#endif  // #ifndef __PINOUT_HEADER_H__

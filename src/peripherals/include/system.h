#ifndef __SYSTEM_HEADER_H__
#define __SYSTEM_HEADER_H__

// Header Inclusions ---------------------------------------------------------------------------------------------------

#include "app_config.h"


// Public API Functions ------------------------------------------------------------------------------------------------

void setup_hardware(void);
void system_reset(void);
void system_enable_interrupts(bool enabled);
void system_enter_deep_sleep_mode(void);
void system_delay(uint32_t delay_us);

#endif  // #ifndef __SYSTEM_HEADER_H__

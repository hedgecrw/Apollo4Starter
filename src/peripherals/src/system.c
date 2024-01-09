// Header Inclusions ---------------------------------------------------------------------------------------------------

#include "logging.h"
#include "system.h"


// Ambiq Interrupt Service Routines and MCU Functions ------------------------------------------------------------------

void _close(void) {}
void _lseek(void) {}
void _read(void) {}
void _write(void) {}
void _fstat(void) {}
void _getpid(void) {}
void _isatty(void) {}
void _kill(void) {}

void am_gpio0_001f_isr(void)
{
   static uint32_t status;
   AM_CRITICAL_BEGIN
   am_hal_gpio_interrupt_irq_status_get(GPIO0_001F_IRQn, false, &status);
   am_hal_gpio_interrupt_irq_clear(GPIO0_001F_IRQn, status);
   AM_CRITICAL_END
   am_hal_gpio_interrupt_service(GPIO0_001F_IRQn, status);
}

void am_gpio0_203f_isr(void)
{
   static uint32_t status;
   AM_CRITICAL_BEGIN
   am_hal_gpio_interrupt_irq_status_get(GPIO0_203F_IRQn, false, &status);
   am_hal_gpio_interrupt_irq_clear(GPIO0_203F_IRQn, status);
   AM_CRITICAL_END
   am_hal_gpio_interrupt_service(GPIO0_203F_IRQn, status);
}

void am_gpio0_405f_isr(void)
{
   static uint32_t status;
   AM_CRITICAL_BEGIN
   am_hal_gpio_interrupt_irq_status_get(GPIO0_405F_IRQn, false, &status);
   am_hal_gpio_interrupt_irq_clear(GPIO0_405F_IRQn, status);
   AM_CRITICAL_END
   am_hal_gpio_interrupt_service(GPIO0_405F_IRQn, status);
}

void am_gpio0_607f_isr(void)
{
   static uint32_t status;
   AM_CRITICAL_BEGIN
   am_hal_gpio_interrupt_irq_status_get(GPIO0_607F_IRQn, false, &status);
   am_hal_gpio_interrupt_irq_clear(GPIO0_607F_IRQn, status);
   AM_CRITICAL_END
   am_hal_gpio_interrupt_service(GPIO0_607F_IRQn, status);
}

void am_rtc_isr(void)
{
   static am_hal_rtc_alarm_repeat_e repeat_interval;
   AM_CRITICAL_BEGIN
   am_hal_rtc_alarm_get(NULL, &repeat_interval);
   am_hal_rtc_interrupt_clear(AM_HAL_RTC_INT_ALM);
   AM_CRITICAL_END
}


// Helpful Debugging Functions and Macros ------------------------------------------------------------------------------

typedef struct __attribute__((packed)) ContextStateFrame
{ uint32_t r0, r1, r2, r3, r12, lr, return_address, xpsr; } sContextStateFrame;

#define HARDFAULT_HANDLING_ASM(_x)               \
  __asm volatile(                                \
      "tst lr, #4 \n"                            \
      "ite eq \n"                                \
      "mrseq r0, msp \n"                         \
      "mrsne r0, psp \n"                         \
      "b system_hard_fault_handler \n"           )

__attribute__((optimize("O0")))
void system_hard_fault_handler(sContextStateFrame *frame)
{
#ifdef DEBUGGING
   do {
      if (CoreDebug->DHCSR & (1 << 0))
         __asm("bkpt 1");
   } while (0);
#else
   NVIC_SystemReset();
   while (true) {}
#endif
}

void HardFault_Handler(void) { HARDFAULT_HANDLING_ASM(); }

void vAssertCalled(const char * const pcFileName, unsigned long ulLine)
{
   volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
   while (ulSetToNonZeroInDebuggerToContinue == 0);
}


// Public API Functions ------------------------------------------------------------------------------------------------

void setup_hardware(void)
{
   // Read the hardware reset reason
   am_hal_reset_status_t reset_reason;
   am_hal_reset_status_get(&reset_reason);

   // Enable the floating point module
   am_hal_sysctrl_fpu_enable();
   am_hal_sysctrl_fpu_stacking_enable(true);

   // Configure the board to operate in low-power mode
   am_hal_pwrctrl_low_power_init();
   am_hal_pwrctrl_control(AM_HAL_PWRCTRL_CONTROL_SIMOBUCK_INIT, NULL);

   // Configure only the necessary memory
   am_hal_pwrctrl_dsp_memory_config_t dsp_mem_config =
   {
      .bEnableICache = false,
      .bRetainCache = false,
      .bEnableRAM = false,
      .bActiveRAM = false,
      .bRetainRAM = false
   };
   am_hal_pwrctrl_mcu_memory_config_t mcu_mem_config =
   {
      .eCacheCfg    = AM_HAL_PWRCTRL_CACHE_NONE,
      .bRetainCache = false,
      .eDTCMCfg     = AM_HAL_PWRCTRL_DTCM_384K,
      .eRetainDTCM  = AM_HAL_PWRCTRL_DTCM_384K,
      .bEnableNVM0  = true,
      .bRetainNVM0  = false
   };
   am_hal_pwrctrl_sram_memcfg_t sram_mem_config =
   {
      .eSRAMCfg           = AM_HAL_PWRCTRL_SRAM_NONE,
      .eActiveWithMCU     = AM_HAL_PWRCTRL_SRAM_NONE,
      .eActiveWithGFX     = AM_HAL_PWRCTRL_SRAM_NONE,
      .eActiveWithDISP    = AM_HAL_PWRCTRL_SRAM_NONE,
      .eActiveWithDSP     = AM_HAL_PWRCTRL_SRAM_NONE,
      .eSRAMRetain        = AM_HAL_PWRCTRL_SRAM_NONE
   };
   am_hal_pwrctrl_dsp_memory_config(AM_HAL_DSP0, &dsp_mem_config);
   am_hal_pwrctrl_dsp_memory_config(AM_HAL_DSP1, &dsp_mem_config);
   am_hal_pwrctrl_mcu_memory_config(&mcu_mem_config);
   am_hal_pwrctrl_sram_config(&sram_mem_config);
   am_hal_cachectrl_disable();

   // Set up printing to the console
   logging_init();
   print_reset_reason(&reset_reason);
}

void system_reset(void)
{
   am_hal_reset_control(AM_HAL_RESET_CONTROL_SWPOR, NULL);
}

void system_enable_interrupts(bool enabled)
{
   // Enable or disable all system interrupts
   if (enabled)
      am_hal_interrupt_master_enable();
   else
      am_hal_interrupt_master_disable();
}

void system_enter_deep_sleep_mode(void)
{
   // Enable interrupts and enter Deep Sleep mode
   am_hal_interrupt_master_enable();
   am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
}

void system_delay(uint32_t delay_us)
{
   am_hal_delay_us(delay_us);
}
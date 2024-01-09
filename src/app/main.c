#include "logging.h"
#include "system.h"

int main(void)
{
   // Set up the system hardware
   setup_hardware();

   // Initialize needed peripherals
   system_enable_interrupts(true);

   // Loop forever
   while (true)
   {
      // Sleep until woken by an interrupt
      print("Going to sleep...\n");
      system_enter_deep_sleep_mode();
      print("Woke up!\n");
   }

   // Should never reach this point
   system_reset();
   return 0;
}

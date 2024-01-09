#include "logging.h"
#include "system.h"

static uint8_t uid[EUI_LEN];

int main(void)
{
   // Set up the system hardware and retrieve the device ID
   setup_hardware();
   system_read_UID(uid, sizeof(uid));
   print("System initialized, UID = %X", uid[0]);
   for (size_t i = 1; i < EUI_LEN; ++i)
      print(":%02X", uid[i]);
   print("\n");

   // Put the CPU into deep sleep forever
   while (true)
      am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);

   // Should never reach this point
   return 0;
}

#include "core_hal.h"
#include "rtc_hal.h"
#include "wiring_wifi.h"
#include "wiring_cloud.h"
#include "system_task.h"
#include "system_network.h"
#include "wiring_system.h"


SystemClass System;

void SystemClass::factoryReset(void)
{
    HAL_Core_Enter_Factory_Reset_Mode();
}

void SystemClass::dfu(bool persist)
{
    // true  - DFU mode persist if firmware upgrade is not completed
    // false - Briefly enter DFU bootloader mode (works with latest bootloader only )
    //         Subsequent reset or power off-on will execute normal firmware
    HAL_Core_Enter_Bootloader(persist);
}

void SystemClass::reset(void)
{
    HAL_Core_System_Reset();
}

void SystemClass::sleep(Spark_Sleep_TypeDef sleepMode, long seconds)
{
    system_sleep(sleepMode, seconds, 0, NULL);
}

void SystemClass::sleep(uint16_t wakeUpPin, InterruptMode edgeTriggerMode, long seconds)
{
    system_sleep_pin(wakeUpPin, edgeTriggerMode, seconds, 0, NULL);
}

uint32_t SystemClass::freeMemory()
{
    runtime_info_t info;
    memset(&info, 0, sizeof(info));
    info.size = sizeof(info);
    HAL_Core_Runtime_Info(&info, NULL);
    return info.freeheap;
}
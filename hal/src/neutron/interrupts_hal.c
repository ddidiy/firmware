/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "interrupts_hal.h"
#include "hw_config.h"


void HAL_Interrupts_Attach(uint16_t pin, HAL_InterruptHandler handler, void* data, InterruptMode mode,
        HAL_InterruptExtraConfiguration* config)
{
}

void HAL_Interrupts_Detach(uint16_t pin)
{
}

void HAL_Interrupts_Enable_All(void)
{
}

void HAL_Interrupts_Disable_All(void)
{
}

void HAL_Interrupts_Suspend(void)
{
}

void HAL_Interrupts_Restore(void)
{
}

/*******************************************************************************
 * Function Name  : HAL_EXTI_Handler (Declared as weak in stm32_it.cpp)
 * Description    : This function is called by any of the interrupt handlers. It
                                                 essentially fetches the user function pointer from the array
                                                 and calls it.
 * Input          : EXTI_Line (Supported: 0, 1, 3, 4, 5, 6, 7, 13, 14 and 15)
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void HAL_EXTI_Handler(uint8_t EXTI_Line)
{
}


int HAL_disable_irq()
{
    return 0;
}

void HAL_enable_irq(int is)
{
}

inline bool isISR()
{
	return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

uint8_t HAL_IsISR()
{
	return isISR();
}

/* For now, we remember only one handler, but in future this may be extended to a
 * dynamically linked list to allow for multiple handlers.
 */
static HAL_InterruptCallback SystemInterruptHandlers[__Last_irq];


inline bool is_valid_irq(hal_irq_t irq)
{
    return irq<__Last_irq;
}

uint8_t HAL_Set_System_Interrupt_Handler(hal_irq_t irq, const HAL_InterruptCallback* callback, HAL_InterruptCallback* previous, void* reserved)
{
    if (!is_valid_irq(irq))
        return false;
    HAL_InterruptCallback cb = SystemInterruptHandlers[irq];
    if (previous)
        *previous = cb;
    if (callback)
        cb = *callback;
    else
        cb.handler = 0;

    return true;
}

uint8_t HAL_Get_System_Interrupt_Handler(hal_irq_t irq, HAL_InterruptCallback* callback, void* reserved)
{
    if (!is_valid_irq(irq))
        return false;

    if (callback) {
        HAL_InterruptCallback cb = SystemInterruptHandlers[irq];
        *callback = cb;
    }

    return true;
}

void HAL_System_Interrupt_Trigger(hal_irq_t irq, void* reserved)
{
    if (is_valid_irq(irq))
    {
        HAL_InterruptCallback cb = SystemInterruptHandlers[irq];
        if (cb.handler)
            cb.handler(cb.data);
    }
}


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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_HAL_H
#define __RTC_HAL_H

#include <stdint.h>
#include <time.h>

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void HAL_RTC_Initial(void);
double HAL_RTC_Get_Tick_Duration(void);
double HAL_RTC_Get_Tick_Per_Ms(void);
time_t HAL_RTC_Get_UnixTime(void);
void HAL_RTC_Set_UnixTime(time_t value);
void HAL_RTC_Set_UnixAlarm(time_t value);
void HAL_RTC_Cancel_UnixAlarm(void);
uint8_t HAL_RTC_Time_Is_Valid(void* reserved);
void HAL_RTC_SetCallbacks(const void (*handle)(), void* reserved);

#ifdef __cplusplus
}
#endif

#endif  /* __RTC_HAL_H */

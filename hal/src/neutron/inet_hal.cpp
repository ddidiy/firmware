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


#include "inet_hal.h"
#include "parser.h"

int inet_gethostbyname(const char* hostname, uint16_t hostnameLen, HAL_IPAddress* out_ip_addr,
        network_interface_t nif, void* reserved)
{
    MDM_IP ip;

    ip = esp8266MDM.getHostByName(hostname);
    if(NOIP != ip) {
        out_ip_addr->ipv4 = ip;
        return 0;
    }
    return -1;
}

int inet_ping(const HAL_IPAddress* address, network_interface_t nif, uint8_t nTries,
        void* reserved)
{
    int count = 0;
    for (int i=0; i<nTries; i++) {
        if(true==esp8266MDM.ping(address->ipv4)){
            count++;
        }
    }
    return count;
}

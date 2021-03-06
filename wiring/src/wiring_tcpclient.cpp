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

#include "intorobot_config.h"

#ifndef configNO_NETWORK

#include "wiring_tcpclient.h"
#include "wiring_network.h"
#include "system_task.h"
#include "socket_hal.h"
#include "inet_hal.h"
#include "intorobot_macros.h"

//#define WIRING_TCPCLIENT_DEBUG

#ifdef WIRING_TCPCLIENT_DEBUG
#define WTCPCLIENT_DEBUG(...)    do {DEBUG(__VA_ARGS__);}while(0)
#define WTCPCLIENT_DEBUG_D(...)  do {DEBUG_D(__VA_ARGS__);}while(0)
#define WTCPCLIENT_DEBUG_DUMP    DEBUG_DUMP
#else
#define WTCPCLIENT_DEBUG(...)
#define WTCPCLIENT_DEBUG_D(...)
#define WTCPCLIENT_DEBUG_DUMP
#endif

using namespace intorobot;

uint16_t TCPClient::_srcport = 1024;

static bool inline isOpen(sock_handle_t sd)
{
    return socket_handle_valid(sd);
}

TCPClient::TCPClient() : TCPClient(socket_handle_invalid())
{
}

TCPClient::TCPClient(sock_handle_t sock) : _sock(sock)
{
    flush_buffer();
}

int TCPClient::connect(const char* host, uint16_t port, network_interface_t nif)
{
    stop();
    int rv = 0;
    if(Network.ready())
    {
        IPAddress ip_addr;

        if((rv = inet_gethostbyname(host, strlen(host), ip_addr, nif, NULL)) == 0)
        {
            return connect(ip_addr, port, nif);
        }
        else{
            WTCPCLIENT_DEBUG("tcp connect: unable to get IP for hostname\r\n");
        }
    }
    return rv;
}

int TCPClient::connect(IPAddress ip, uint16_t port, network_interface_t nif)
{
    stop();
    int connected = 0;
    if(Network.from(nif).ready())
    {
        sockaddr_t tSocketAddr;
        _sock = socket_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, port, nif);

        if (socket_handle_valid(_sock))
        {
            flush_buffer();
            tSocketAddr.sa_family = AF_INET;
            tSocketAddr.sa_data[0] = (port & 0xFF00) >> 8;
            tSocketAddr.sa_data[1] = (port & 0x00FF);
            tSocketAddr.sa_data[2] = ip[0];        // Todo IPv6
            tSocketAddr.sa_data[3] = ip[1];
            tSocketAddr.sa_data[4] = ip[2];
            tSocketAddr.sa_data[5] = ip[3];

            uint32_t ot = HAL_NET_SetNetWatchDog(S2M(MAX_SEC_WAIT_CONNECT));
            connected = (socket_connect(_sock, &tSocketAddr, sizeof(tSocketAddr)) == 0 ? 1 : 0);
            HAL_NET_SetNetWatchDog(ot);
            _remoteIP = ip;
            if(connected)
            {
                WTCPCLIENT_DEBUG("tcp connect success! create socket %d\r\n", _sock);
            }
            else
            {
                WTCPCLIENT_DEBUG("tcp connect failed!\r\n");
                stop();
            }
        }
    }
    return connected;
}

size_t TCPClient::write(uint8_t b)
{
    return write(&b, 1);
}

size_t TCPClient::write(const uint8_t *buffer, size_t size)
{
    return status() ? socket_send(_sock, buffer, size) : -1;
}

int TCPClient::bufferCount()
{
    return _total - _offset;
}

int TCPClient::available()
{
    int avail = 0;

    // At EOB => Flush it
    if (_total && (_offset == _total))
    {
        flush_buffer();
    }

    if(Network.from(nif).ready() && isOpen(_sock))
    {
        // Have room
        if ( _total < arraySize(_buffer))
        {
            int ret = socket_receive(_sock, _buffer + _total , arraySize(_buffer)-_total, 0);
            if (ret > 0)
            {
                WTCPCLIENT_DEBUG("tcp receive data %d\r\n", ret);
                if (_total == 0) _offset = 0;
                _total += ret;
            }
        } // Have Space
    } // WiFi.ready() && isOpen(_sock)
    avail = bufferCount();
    return avail;
}

int TCPClient::read()
{
    return (bufferCount() || available()) ? _buffer[_offset++] : -1;
}

int TCPClient::read(uint8_t *buffer, size_t size)
{
    int read = -1;
    if (bufferCount() || available())
    {
        read = (size > (size_t) bufferCount()) ? bufferCount() : size;
        memcpy(buffer, &_buffer[_offset], read);
        _offset += read;
    }
    return read;
}

int TCPClient::peek()
{
    return  (bufferCount() || available()) ? _buffer[_offset] : -1;
}

void TCPClient::flush_buffer()
{
    _offset = 0;
    _total = 0;
}

void TCPClient::flush()
{
    while (available())
        read();
}


void TCPClient::stop()
{
    WTCPCLIENT_DEBUG("tcp stop! close socket %d\r\n", _sock);
    if (isOpen(_sock))
        socket_close(_sock);
    _sock = socket_handle_invalid();
    _remoteIP.clear();
    flush_buffer();
}

uint8_t TCPClient::connected()
{
    // Wlan up, open and not in CLOSE_WAIT or data still in the local buffer
    bool rv = (status() || bufferCount());
    // no data in the local buffer, Socket open but my be in CLOSE_WAIT yet the CC3000 may have data in its buffer
    if(!rv && isOpen(_sock) && (SOCKET_STATUS_INACTIVE == socket_active_status(_sock)))
    {
        rv = available(); // Try CC3000
        if (!rv) {        // No more Data and CLOSE_WAIT
            // DEBUG("caling Stop No more Data and in CLOSE_WAIT\r\n");
            stop();       // Close our side
        }
    }
    return rv;
}

uint8_t TCPClient::status()
{
    return (isOpen(_sock) && Network.from(nif).ready() && (SOCKET_STATUS_ACTIVE == socket_active_status(_sock)));
}

TCPClient::operator bool()
{
    return (status()!=0);
}

IPAddress TCPClient::remoteIP()
{
    return _remoteIP;
}

#endif

/**
 * @file ESP8266.c
 * @brief The implementation of class ESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.02
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/*---------------------------------------------------------------------------------------------------------*/
/* Header File                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#include "esp8266.h"

#include <string.h>
#include <stdlib.h>
/*---------------------------------------------------------------------------------------------------------*/
/* Defines                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define ESP8266_PORT	UART2
#define ESP8266_BAUD	115200

#define ESP8266_TIMER	TIMER2

#define ESP8266_BUF_SIZE	512
/*---------------------------------------------------------------------------------------------------------*/
/* Function Prototype                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t esp8266_recvPkg(uint8_t *buffer, uint32_t buffer_size, uint32_t *data_len, uint32_t timeout, uint8_t *coming_mux_id);

static void esp8266_uart_send(char* cmd);
static void esp8266_uart_recv_clear(void);
static uint32_t esp8266_uart_recv(uint32_t timeout);
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t esp8266_rx_buf[ESP8266_BUF_SIZE];
/*---------------------------------------------------------------------------------------------------------*/
/* Functions                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
//void esp8266_init(void)
//{

//    /* Configure UART0 and set UART0 baud rate */
//    UART_Open(ESP8266_PORT, ESP8266_BAUD);
//    
////	CLK->APBCLK0 |= CLK_APBCLK0_UART2CKEN_Msk;
////    CLK->CLKSEL3 = (CLK->CLKSEL3 & (~CLK_CLKSEL3_UART2SEL_Msk)) | CLK_CLKSEL3_UART2SEL_HIRC;

//    /* Set multi-function pins for RXD and TXD */
//    SET_UART2_RXD_PC0();
//    SET_UART2_TXD_PC1();
//    
//    /* Init Timer tick (1tick = 10ms) */
//    ESP8266_TIMER->CTL = TIMER_CONTINUOUS_MODE;
//    TIMER_SET_PRESCALE_VALUE(ESP8266_TIMER, 0);
//    TIMER_SET_CMP_VALUE(ESP8266_TIMER, 0xFFFFFF);
//    TIMER_Start(ESP8266_TIMER);
//}

/** 
 * Verify ESP8266 whether live or not. 
 *
 * Actually, this method will send command "AT" to ESP8266 and waiting for "OK". 
 * 
 * @retval 1 - alive.
 * @retval 0 - dead.
 */
uint8_t esp8266_kick(void)
{
	return esp8266_eAT();
}
/**
 * Restart ESP8266 by "AT+RST". 
 *
 * This method will take 3 seconds or more. 
 *
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_restart(void)
{
	uint32_t start;
	
    if (esp8266_eATRST()) {
        CLK_SysTickDelay(100000);
        start = TIMER_GetCounter(ESP8266_TIMER);
        while ((TIMER_GetCounter(ESP8266_TIMER) - start) < 3000) {
            if (esp8266_eAT()) {
                CLK_SysTickDelay(100000);
                return 1;
            }
            CLK_SysTickDelay(100000);
        }
    }
    return 0;
}
/**
 * Get the version of AT Command Set. 
 * @param ver - buffer to store version. 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_getVersion(char* ver)
{
	return esp8266_eATGMR(ver);
}
/**
 * Set operation mode to staion. 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_setOprToStation(void)
{
    uint8_t mode;
    if (!esp8266_qATCWMODE(&mode)) {
        return 0;
    }
    if (mode == 1) {
        return 1;
    } else {
        if (esp8266_sATCWMODE(1) && esp8266_restart()) {
            return 1;
        } else {
            return 0;
        }
    }
}
/**
 * Set operation mode to softap. 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_setOprToSoftAP(void)
{
    uint8_t mode;
    if (!esp8266_qATCWMODE(&mode)) {
        return 0;
    }
    if (mode == 2) {
        return 1;
    } else {
        if (esp8266_sATCWMODE(2) && esp8266_restart()) {
            return 1;
        } else {
            return 0;
        }
    }
}
/**
 * Set operation mode to station + softap. 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_setOprToStationSoftAP(void)
{
    uint8_t mode;
    if (!esp8266_qATCWMODE(&mode)) {
        return 0;
    }
    if (mode == 3) {
        return 1;
    } else {
        if (esp8266_sATCWMODE(3) && esp8266_restart()) {
            return 1;
        } else {
            return 0;
        }
    }
}

/**
 * Search available AP list .
 * @param ver - buffer to store AP list. 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 * @note This method will occupy a lot of memeory(hundreds of Bytes to a couple of KBytes). 
 *  Do not call this method unless you must and ensure that your board has enough memery left.
 */
uint8_t esp8266_getAPList(char* list)
{
    return esp8266_eATCWLAP(list);
}
/**
 * Join in AP. 
 *
 * @param ssid - SSID of AP to join in. 
 * @param pwd - Password of AP to join in. 
 * @retval 1 - success.
 * @retval 0 - failure.
 * @note This method will take a couple of seconds. 
 */
uint8_t esp8266_joinAP(char* ssid, char* pwd)
{
    return esp8266_sATCWJAP(ssid, pwd);
}
/**
 * Enable DHCP for client mode. 
 *
 * @param mode - server mode (0=soft AP, 1=station, 2=both
 * @param enabled - true if dhcp should be enabled, otherwise false
 * 
 * @note This method will enable DHCP but only for client mode!
 */
uint8_t esp8266_enableClientDHCP(uint8_t mode, uint8_t enabled)
{
	char en = (char) enabled;
	
    return esp8266_sATCWDHCP(mode, &en);
}
/**
 * Leave AP joined before. 
 *
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_leaveAP(void)
{
    return esp8266_eATCWQAP();
}
/**
 * Set SoftAP parameters. 
 * 
 * @param ssid - SSID of SoftAP. 
 * @param pwd - PASSWORD of SoftAP. 
 * @param chl - the channel (1 - 13, default: 7). 
 * @param ecn - the way of encrypstion (0 - OPEN, 1 - WEP, 
 *  2 - WPA_PSK, 3 - WPA2_PSK, 4 - WPA_WPA2_PSK, default: 4). 
 * @note This method should not be called when station mode. 
 */
uint8_t esp8266_setSoftAPParam(char* ssid, char* pwd, uint8_t chl, uint8_t ecn)
{
    return esp8266_sATCWSAP(ssid, pwd, chl, ecn);
}
/**
 * Get the IP list of devices connected to SoftAP. 
 * @param list - the list of IP.
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 * @note This method should not be called when station mode. 
 */
uint8_t esp8266_getJoinedDeviceIP(char* list)
{
    return esp8266_eATCWLIF(list);
}
/**
 * Get the current status of connection(UDP and TCP). 
 * @param list - the status.
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_getIPStatus(char* list)
{
	return esp8266_eATCIPSTATUS(list);
}
/**
 * Get the IP address of ESP8266. 
 * @param list - the IP list.
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_getLocalIP(char* list)
{
	return esp8266_eATCIFSR(list);
}
/**
 * Enable IP MUX(multiple connection mode). 
 *
 * In multiple connection mode, a couple of TCP and UDP communication can be builded. 
 * They can be distinguished by the identifier of TCP or UDP named mux_id. 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_enableMUX(void)
{
    return esp8266_sATCIPMUX(1);
}
/**
 * Disable IP MUX(single connection mode). 
 *
 * In single connection mode, only one TCP or UDP communication can be builded. 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_disableMUX(void)
{
    return esp8266_sATCIPMUX(0);
}
/**
 * Create TCP connection in single mode. 
 * 
 * @param addr - the IP or domain name of the target host. 
 * @param port - the port number of the target host. 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_createTCP(char* addr, uint32_t port)
{
    return esp8266_sATCIPSTARTSingle("TCP", addr, port);
}
/**
 * Release TCP connection in single mode. 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_releaseTCP(void)
{
    return esp8266_eATCIPCLOSESingle();
}
/**
 * Register UDP port number in single mode.
 * 
 * @param addr - the IP or domain name of the target host. 
 * @param port - the port number of the target host. 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_registerUDP(char* addr, uint32_t port)
{
    return esp8266_sATCIPSTARTSingle("UDP", addr, port);
}
/**
 * Unregister UDP port number in single mode. 
 * 
 * @retval true - success.
 * @retval false - failure.
 */
uint8_t esp8266_unregisterUDP(void)
{
    return esp8266_eATCIPCLOSESingle();
}
/**
 * Create TCP connection in multiple mode. 
 * 
 * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
 * @param addr - the IP or domain name of the target host. 
 * @param port - the port number of the target host. 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_createTCP_Multiple(uint8_t mux_id, char* addr, uint32_t port)
{
    return esp8266_sATCIPSTARTMultiple(mux_id, "TCP", addr, port);
}
/**
 * Release TCP connection in multiple mode. 
 * 
 * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_releaseTCP_Multiple(uint8_t mux_id)
{
    return esp8266_sATCIPCLOSEMulitple(mux_id);
}
/**
 * Register UDP port number in multiple mode.
 * 
 * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
 * @param addr - the IP or domain name of the target host. 
 * @param port - the port number of the target host. 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_registerUDP_Multiple(uint8_t mux_id, char* addr, uint32_t port)
{
    return esp8266_sATCIPSTARTMultiple(mux_id, "UDP", addr, port);
}
/**
 * Unregister UDP port number in multiple mode. 
 * 
 * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_unregisterUDP_Multiple(uint8_t mux_id)
{
    return esp8266_sATCIPCLOSEMulitple(mux_id);
}
/**
 * Set the timeout of TCP Server. 
 * 
 * @param timeout - the duration for timeout by second(0 ~ 28800, default:180). 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_setTCPServerTimeout(uint32_t timeout)
{
    return esp8266_sATCIPSTO(timeout);
}
/**
 * Start TCP Server(Only in multiple mode). 
 * 
 * After started, user should call method: getIPStatus to know the status of TCP connections. 
 * The methods of receiving data can be called for user's any purpose. After communication, 
 * release the TCP connection is needed by calling method: releaseTCP with mux_id. 
 *
 * @param port - the port number to listen(default: 333).
 * @retval true - success.
 * @retval false - failure.
 *
 * @see String getIPStatus(void);
 * @see uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t len, uint32_t timeout);
 * @see bool releaseTCP(uint8_t mux_id);
    */
uint8_t esp8266_startTCPServer(uint32_t port)
{
    if (esp8266_sATCIPSERVER(1, port)) {
        return 1;
    }
    return 0;
}
/**
 * Stop TCP Server(Only in multiple mode). 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_stopTCPServer(void)
{
    esp8266_sATCIPSERVER(0, 0);
    return esp8266_restart();
}
/**
 * Start Server(Only in multiple mode). 
 * 
 * @param port - the port number to listen(default: 333).
 * @retval 1 - success.
 * @retval 0 - failure.
 *
 * @see String getIPStatus(void);
 * @see uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t len, uint32_t timeout);
 */
uint8_t esp8266_startServer(uint32_t port)
{
    return esp8266_startTCPServer(port);
}
/**
 * Stop Server(Only in multiple mode). 
 * 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_stopServer(void)
{
    return esp8266_stopTCPServer();
}
/**
 * Send data based on TCP or UDP builded already in single mode. 
 * 
 * @param buffer - the buffer of data to send. 
 * @param len - the length of data to send. 
 * @retval 1 - success.
 * @retval 0 - failure.
 */
uint8_t esp8266_send(const uint8_t *buffer, uint32_t len)
{
    return esp8266_sATCIPSENDSingle(buffer, len);
}
/**
 * Send data based on one of TCP or UDP builded already in multiple mode. 
 * 
 * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
 * @param buffer - the buffer of data to send. 
 * @param len - the length of data to send. 
 * @retval true - success.
 * @retval false - failure.
 */
uint8_t esp8266_send_Multiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
    return esp8266_sATCIPSENDMultiple(mux_id, buffer, len);
}
/**
 * Receive data from TCP or UDP builded already in single mode. 
 *
 * @param buffer - the buffer for storing data. 
 * @param buffer_size - the length of the buffer. 
 * @param timeout - the time waiting data. 
 * @return the length of data received actually. 
 */
uint32_t esp8266_recv(uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    return esp8266_recvPkg(buffer, buffer_size, NULL, timeout, NULL);
}
/**
 * Receive data from all of TCP or UDP builded already in multiple mode. 
 *
 * After return, coming_mux_id store the id of TCP or UDP from which data coming. 
 * User should read the value of coming_mux_id and decide what next to do. 
 * 
 * @param coming_mux_id - the identifier of TCP or UDP. 
 * @param buffer - the buffer for storing data. 
 * @param buffer_size - the length of the buffer. 
 * @param timeout - the time waiting data. 
 * @return the length of data received actually. 
*/
uint32_t esp8266_recv_Multiple(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    return esp8266_recvPkg(buffer, buffer_size, NULL, timeout, coming_mux_id);
}

/****************************************************/
/*----------------------------------------------------------------------------*/
/* +IPD,<id>,<len>:<data> */
/* +IPD,<len>:<data> */

static uint32_t esp8266_recvPkg(uint8_t *buffer, uint32_t buffer_size, uint32_t *data_len, uint32_t timeout, uint8_t *coming_mux_id)
{
	char tmp[5];
	char rx_buf[256] = {0};
	uint32_t start;
	uint32_t i = 0;
	uint32_t a;
	
	char* index_PIPDcomma = NULL;
    char* index_colon = NULL; /* : */
    char* index_comma = NULL; /* , */
	
	int8_t id = -1;
	uint32_t len = 0;
	uint8_t has_data = 0;

    if (buffer == NULL) {
        return 0;
    }
	
	start = TIMER_GetCounter(ESP8266_TIMER);
	
	while (TIMER_GetCounter(ESP8266_TIMER) - start < timeout) {
		while(UART_GET_RX_EMPTY(ESP8266_PORT) == 0) {
			a = UART_READ(ESP8266_PORT);
			rx_buf[i++] = (uint8_t) a;
		}
	}
	
	if (i > 0) {
			index_PIPDcomma = strstr((const char*)rx_buf, "+IPD,");
			if (index_PIPDcomma != NULL) {
				index_colon = strstr((const char*)(index_PIPDcomma + 5), ":");
				if (index_colon != NULL) {
					index_comma = strstr((const char*)(index_PIPDcomma + 5), ",");
					/* +IPD,id,len:data */
					if ( (index_comma != NULL) && (index_comma < index_colon)) {
						id = *(index_PIPDcomma + 5) - 0x30;	// ASCII to Int
						if (id < 0 || id > 4) {
							return 0;
						}
						memcpy(tmp, (index_comma + 1), sizeof(char) * (index_colon - (index_comma + 1)));
						len = atoi(tmp);
						//sprintf(tmp, "%d", len);
						if (len <= 0) {
							return 0;
						}
					} else {	/* +IPD,len:data */
						memcpy(tmp, (index_PIPDcomma + 5), sizeof(char) * (index_colon - (index_PIPDcomma + 5)));
						len = atoi(tmp);
						if (len <= 0) {
							return 0;
						}	
					}
					has_data = 1;	
				}	
			}	
	}
	
	if (has_data) {
		if (data_len) {
			*data_len = len;    
		}
		if ((index_comma != NULL) && coming_mux_id) {
			*coming_mux_id = id;
		}
		
		memcpy(buffer, (index_colon + 1), sizeof(char) * len);
		return len;
	}
	
	return 0;
}

uint8_t esp8266_eAT(void)
{
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    esp8266_uart_send("AT\r\n");
	esp8266_uart_recv(100);
	
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	return 1;
}

uint8_t esp8266_eATRST(void) 
{
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    esp8266_uart_send("AT+RST\r\n");
	esp8266_uart_recv(100);
	
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	return 1;
}

uint8_t esp8266_eATGMR(char* ver)
{
	uint8_t recv;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+GMR\r\n");
	recv = esp8266_uart_recv(1000);
	
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	memcpy(ver, esp8266_rx_buf, sizeof(char) * recv);
	
	return 1;
}

uint8_t esp8266_qATCWMODE(uint8_t *mode) 
{
	char* index = 0;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    if (!mode) {
        return 0;
    }
	
    esp8266_uart_send("AT+CWMODE?\r\n");
	esp8266_uart_recv(100);
	
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	index = strstr((const char*)esp8266_rx_buf, "+CWMODE:");
	*mode = (uint8_t)(*(index + 8) - 0x30);
	
	return 1;
}

uint8_t esp8266_sATCWMODE(uint8_t mode)
{
	char tmp = (char)mode + 0x30;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CWMODE=");
	esp8266_uart_send(&tmp);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(100);

	if ((strstr((const char*)esp8266_rx_buf, "OK") == NULL) || \
		((strstr((const char*)esp8266_rx_buf, "no change") == NULL)))
		return 0;
	
	return 1;
}

uint8_t esp8266_sATCWJAP(char* ssid, char* pwd)
{
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CWJAP=\"");
	esp8266_uart_send(ssid);
    esp8266_uart_send("\",\"");
    esp8266_uart_send(pwd);
    esp8266_uart_send("\"");
	esp8266_uart_send("\r\n");

	esp8266_uart_recv(50000);
	
	if ((strstr((const char*)esp8266_rx_buf, "OK") == NULL) && \
		(strstr((const char*)esp8266_rx_buf, "CONNECTED") == NULL))
		return 0;
	
	return 1;
}

uint8_t esp8266_sATCWDHCP(uint8_t mode, char* enabled)
{
	char tmp = mode + 0x30;	// change to ASCII
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    esp8266_uart_send("AT+CWDHCP=");
    esp8266_uart_send(enabled);
    esp8266_uart_send(",");
    esp8266_uart_send(&tmp);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(100);
	
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	return 1;
}

uint8_t esp8266_eATCWLAP(char* list)
{
	uint8_t recv;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CWLAP\r\n");
	recv = esp8266_uart_recv(100);
	
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	memcpy(list, esp8266_rx_buf, sizeof(char) * recv);

	return 1;
}

uint8_t esp8266_eATCWQAP(void)
{
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CWQAP\r\n");
	esp8266_uart_recv(100);
	
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	return 1;
}


uint8_t esp8266_sATCWSAP(char* ssid, char* pwd, uint8_t chl, uint8_t ecn)
{
	char channrl = chl;
	char encrpto = ecn;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CWSAP=\"");
	esp8266_uart_send(ssid);
	esp8266_uart_send("\",\"");
    esp8266_uart_send(pwd);
    esp8266_uart_send("\",");
    esp8266_uart_send(&channrl);
    esp8266_uart_send(",");
    esp8266_uart_send(&encrpto);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(500);
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
    
	return 1;
}

uint8_t esp8266_eATCWLIF(char* list)
{
	uint8_t recv;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CWLIF\r\n");
	recv = esp8266_uart_recv(500);
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	memcpy(list, esp8266_rx_buf, sizeof(char) * recv);

	return 1;
}

uint8_t esp8266_eATCIPSTATUS(char* list)
{
	uint8_t recv;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CIPSTATUS\r\n");
	recv = esp8266_uart_recv(500);
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	memcpy(list, esp8266_rx_buf, sizeof(char) * recv);
	
	return 1;
}

uint8_t esp8266_sATCIPSTARTSingle(char* type, char* addr, uint32_t port)
{
	char tmp[4];
	sprintf(tmp, "%d", port);
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);

	esp8266_uart_send("AT+CIPSTART=\"");
	esp8266_uart_send(type);
	esp8266_uart_send("\",\"");
	esp8266_uart_send(addr);
	esp8266_uart_send("\",");
	esp8266_uart_send(tmp);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(10000);
	
	if ((strstr((const char*)esp8266_rx_buf, "OK") == NULL) && \
		(strstr((const char*)esp8266_rx_buf, "ALREADY CONNECT") == NULL))
			
		return 0;

	return 1;
}

uint8_t esp8266_sATCIPSTARTMultiple(uint8_t mux_id, char* type, char* addr, uint32_t port)
{
	char tmp[4];
	char id = mux_id + 0x30;
	
	sprintf(tmp, "%d", port);
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CIPSTART=\"");
	esp8266_uart_send(&id);
	esp8266_uart_send(",\"");
	esp8266_uart_send(type);
	esp8266_uart_send("\",\"");
	esp8266_uart_send(addr);
	esp8266_uart_send("\",");
	esp8266_uart_send(tmp);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(1000);
	
	if ((strstr((const char*)esp8266_rx_buf, "OK") == NULL) && \
		(strstr((const char*)esp8266_rx_buf, "ALREADY CONNECT") == NULL))	
		return 0;

	return 1;
}

uint8_t esp8266_sATCIPSENDSingle(const uint8_t *buffer, uint32_t len)
{
	uint32_t i;
	char tmp;
	char lenArr[4] = {0};
	
	sprintf(lenArr, "%d", len);
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    esp8266_uart_send("AT+CIPSEND=");
    esp8266_uart_send(lenArr);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(1000);
	if (strstr((const char*)esp8266_rx_buf, ">") == NULL)
		return 0;
	
	for (i = 0; i < len; i++) {
		tmp = buffer[i];
		esp8266_uart_send(&tmp);
	}
	
	esp8266_uart_recv(5000);
	if (strstr((const char*)esp8266_rx_buf, "SEND OK") == NULL)
		return 0;

    return 1;
}

uint8_t esp8266_sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
	uint32_t i;
	char tmp;
	char id = mux_id + 0x30;
	char lenArr[4] = {0};
	
	sprintf(lenArr, "%d", len);
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    esp8266_uart_send("AT+CIPSEND=");
    esp8266_uart_send(&id);
    esp8266_uart_send(",");
	esp8266_uart_send(lenArr);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(1000);
	if (strstr((const char*)esp8266_rx_buf, ">") == NULL)
		return 0;
	
	for (i = 0; i < len; i++) {
		tmp = buffer[i];
		esp8266_uart_send(&tmp);
	}
	
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_recv(5000);
	if (strstr((const char*)esp8266_rx_buf, "SEND OK") == NULL)
		return 0;

    return 1;
}

uint8_t esp8266_sATCIPCLOSEMulitple(uint8_t mux_id)
{
	char id = mux_id + 0x30;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    esp8266_uart_send("AT+CIPCLOSE=");
    esp8266_uart_send(&id);
    esp8266_uart_send("\r\n");
	
	if ((strstr((const char*)esp8266_rx_buf, "OK") == NULL) && \
		(strstr((const char*)esp8266_rx_buf, "link is not") == NULL))	
		return 0;
		
	return 1;
}

uint8_t esp8266_eATCIPCLOSESingle(void)
{
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    esp8266_uart_send("AT+CIPCLOSE\r\n");
	esp8266_uart_recv(1000);
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	return 1;
}

uint8_t esp8266_eATCIFSR(char* list)
{
	uint32_t recv;
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
    esp8266_uart_send("AT+CIFSR\r\n");
	recv = esp8266_uart_recv(1000);
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	memcpy(list, esp8266_rx_buf, sizeof(char) * recv);
	
	return 1;
}

uint8_t esp8266_sATCIPMUX(uint8_t mode)
{
	char tmp = mode + 0x30;	// change to ASCII
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CIPMUX=");
	esp8266_uart_send(&tmp);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(1000);
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	return 1;
}

uint8_t esp8266_sATCIPSERVER(uint8_t mode, uint32_t port)
{
	char tmp[4];
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	if (mode) {
		sprintf(tmp, "%d", port);
		
        esp8266_uart_send("AT+CIPSERVER=1,");
        esp8266_uart_send(tmp);
		esp8266_uart_send("\r\n");
		
		esp8266_uart_recv(1000);
		if ((strstr((const char*)esp8266_rx_buf, "OK") == NULL) && \
			(strstr((const char*)esp8266_rx_buf, "no change") == NULL))	
			return 0;
	} else {
        esp8266_uart_send("AT+CIPSERVER=0\r\n");
		esp8266_uart_recv(1000);
	}
	
	return 1;
}

uint8_t esp8266_sATCIPSTO(uint32_t timeout)
{
	char tmp[5];
	sprintf(tmp,"%d",timeout);
	
	esp8266_uart_recv_clear();
	memset(esp8266_rx_buf, 0x00, sizeof(char) * ESP8266_BUF_SIZE);
	
	esp8266_uart_send("AT+CIPSTO=");
	esp8266_uart_send(tmp);
	esp8266_uart_send("\r\n");
	
	esp8266_uart_recv(1000);
	if (strstr((const char*)esp8266_rx_buf, "OK") == NULL)
		return 0;
	
	return 1;
}
/****************************************************/
static void esp8266_uart_send(char* cmd)
{
	UART_Write(ESP8266_PORT, (unsigned char*)cmd, strlen(cmd));
}

static void esp8266_uart_recv_clear(void)
{
	while(UART_GET_RX_EMPTY(ESP8266_PORT) == 0)
		UART_READ(ESP8266_PORT);
}

static uint32_t esp8266_uart_recv(uint32_t timeout)
{
    uint32_t	a;
    uint32_t start, timeout1;
    uint32_t i = 0;

//    start = TIMER_GetCounter(ESP8266_TIMER);
//    while ((TIMER_GetCounter(ESP8266_TIMER) - start) < timeout) {
//        while(UART_GET_RX_EMPTY(ESP8266_PORT) == 0) {
//            a = UART_READ(ESP8266_PORT);
//            if (a == '\0')
//                continue;
//            esp8266_rx_buf[i++] = (uint8_t)a;
//        }
//        
//    }
    timeout1 = timeout * 19200;
    timeout = timeout1;
    while (timeout--) {
        while(UART_GET_RX_EMPTY(ESP8266_PORT) == 0) {
            a = UART_READ(ESP8266_PORT);
            if (a == '\0')
                continue;
            esp8266_rx_buf[i++] = (uint8_t)a;
            timeout = timeout1;
        }
    }
    
    return i;
}

/**
 * @file ESP8266.h
 * @brief The definition of class ESP8266. 
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
#ifndef	__ESP8266_H__
#define __ESP8266_H__

/*---------------------------------------------------------------------------------------------------------*/
/* Header File                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#include "m460.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Defines                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function Prototype                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void esp8266_init(void);

uint8_t esp8266_kick(void);
uint8_t esp8266_restart(void);
uint8_t esp8266_getVersion(char* ver);
uint8_t esp8266_setOprToStation(void);
uint8_t esp8266_setOprToSoftAP(void);
uint8_t esp8266_setOprToStationSoftAP(void);
uint8_t esp8266_getAPList(char* list);
uint8_t esp8266_joinAP(char* ssid, char* pwd);
uint8_t esp8266_enableClientDHCP(uint8_t mode, uint8_t enabled);
uint8_t esp8266_leaveAP(void);
uint8_t esp8266_setSoftAPParam(char* ssid, char* pwd, uint8_t chl, uint8_t ecn);
uint8_t esp8266_getJoinedDeviceIP(char* list);
uint8_t esp8266_getIPStatus(char* list);
uint8_t esp8266_getLocalIP(char* list);
uint8_t esp8266_enableMUX(void);
uint8_t esp8266_disableMUX(void);
uint8_t esp8266_createTCP(char* addr, uint32_t port);
uint8_t esp8266_releaseTCP(void);
uint8_t esp8266_registerUDP(char* addr, uint32_t port);
uint8_t esp8266_unregisterUDP(void);
uint8_t esp8266_createTCP_Multiple(uint8_t mux_id, char* addr, uint32_t port);
uint8_t esp8266_releaseTCP_Multiple(uint8_t mux_id);
uint8_t esp8266_registerUDP_Multiple(uint8_t mux_id, char* addr, uint32_t port);
uint8_t esp8266_unregisterUDP_Multiple(uint8_t mux_id);
uint8_t esp8266_setTCPServerTimeout(uint32_t timeout);
uint8_t esp8266_startTCPServer(uint32_t port);
uint8_t esp8266_startServer(uint32_t port);
uint8_t esp8266_stopServer(void);

uint8_t esp8266_send(const uint8_t *buffer, uint32_t len);
uint8_t esp8266_send_Multiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
uint32_t esp8266_recv(uint8_t *buffer, uint32_t buffer_size, uint32_t timeout);
uint32_t esp8266_recv_Multiple(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout);

uint8_t esp8266_eAT(void);
uint8_t esp8266_eATRST(void);
uint8_t esp8266_eATGMR(char* ver);

uint8_t esp8266_qATCWMODE(uint8_t *mode);
uint8_t esp8266_sATCWMODE(uint8_t mode);
uint8_t esp8266_sATCWJAP(char* ssid, char* pwd);
uint8_t esp8266_sATCWDHCP(uint8_t mode, char* enabled);
uint8_t esp8266_eATCWLAP(char* list);
uint8_t esp8266_eATCWQAP(void);
uint8_t esp8266_sATCWSAP(char* ssid, char* pwd, uint8_t chl, uint8_t ecn);
uint8_t esp8266_eATCWLIF(char* list);

uint8_t esp8266_eATCIPSTATUS(char* list);
uint8_t esp8266_sATCIPSTARTSingle(char* type, char* addr, uint32_t port);
uint8_t esp8266_sATCIPSTARTMultiple(uint8_t mux_id, char* type, char* addr, uint32_t port);
uint8_t esp8266_sATCIPSENDSingle(const uint8_t *buffer, uint32_t len);
uint8_t esp8266_sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
uint8_t esp8266_sATCIPCLOSEMulitple(uint8_t mux_id);
uint8_t esp8266_eATCIPCLOSESingle(void);
uint8_t esp8266_eATCIFSR(char* list);
uint8_t esp8266_sATCIPMUX(uint8_t mode);
uint8_t esp8266_sATCIPSERVER(uint8_t mode, uint32_t port);
uint8_t esp8266_sATCIPSTO(uint32_t timeout);
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

#endif

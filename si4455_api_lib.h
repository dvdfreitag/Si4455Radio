/*!
* File:
*  si4455_api_lib.h
*
* Description:
*  This file contains the Si4455 API library.
*
* Silicon Laboratories Confidential
* Copyright 2011 Silicon Laboratories, Inc.
*/

#ifndef _SI4455_API_LIB_H_
#define _SI4455_API_LIB_H_

#define SI4455_FIFO_SIZE 64

#include <Arduino.h>

enum
{
	SI4455_SUCCESS,
	SI4455_NO_PATCH,
	SI4455_CTS_TIMEOUT,
	SI4455_PATCH_FAIL,
	SI4455_COMMAND_ERROR
};

class tSi4455_API
{	
public:
	/* Minimal driver support functions */
	void Reset();
	void PowerUp(byte BOOT_OPTIONS, byte XTAL_OPTIONS, unsigned long XO_FREQ);
	byte Initialize(const byte* pSetPropCmd);
	void WriteEZConfigArray(byte numBytes, const byte* pEzConfigArray);
	void EZConfigCheck(unsigned int CHECKSUM);
	void StartTX(byte CHANNEL, byte CONDITION, unsigned int TX_LEN);
	void StartRX(byte CHANNEL, byte CONDITION, unsigned int RX_LEN, byte NEXT_STATE1, byte NEXT_STATE2, byte NEXT_STATE3);
	void GetIntStatus(byte PH_CLR_PEND, byte MODEM_CLR_PEND, byte CHIP_CLR_PEND);
	void GpioPinCfg(byte GPIO0, byte GPIO1, byte GPIO2, byte GPIO3, byte NIRQ, byte SDO, byte GEN_CONFIG);
	void SetProperty(byte GROUP, byte NUM_PROPS, byte START_PROP, ...);
	void ChangeState(byte NEXT_STATE1);
};

extern tSi4455_API Si4455_API;

#endif //_SI4455_API_LIB_H_

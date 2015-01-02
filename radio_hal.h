/*!
* File:
*  radio_hal.h
*
* Description:
*  This file contains RADIO HAL.
*
* Silicon Laboratories Confidential
* Copyright 2011 Silicon Laboratories, Inc.
*/

#ifndef _RADIO_HAL_H_
#define _RADIO_HAL_H_

#define SDNPin  1
#define nSELPin 2
#define nIRQPin 3

class tSi4455_Hal
{	
public:
	void AssertShutdown();	
	void DeassertShutdown();
	void ClearNsel();
	void SetNsel();
	boolean NirqLevel();

	void SpiWriteByte(byte byteToWrite);
	byte SpiReadByte();

	void SpiWriteData(byte byteCount, const byte* pData);
	void SpiReadData(byte byteCount, byte* pData);
};

extern tSi4455_Hal Si4455_Hal;

#endif //_RADIO_HAL_H_

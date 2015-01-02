/*!
* File:
*  radio_comm.h
*
* Description:
*  This file contains the RADIO communication layer.
*
* Silicon Laboratories Confidential
* Copyright 2012 Silicon Laboratories, Inc.
*/

#include <Arduino.h>
#include "radio_comm.h"
#include "radio_hal.h"

boolean ctsWentHigh = 0;

tSi4455_RadioComm Si4455_RadioComm = tSi4455_RadioComm();

/*!
* Gets a command response from the radio chip
*
* @param byteCount     Number of bytes to get from the radio chip
* @param pData         Pointer to where to put the data
*
* @return CTS value
*/
byte tSi4455_RadioComm::GetResp(byte byteCount, byte* pData)
{
	byte ctsVal = 0u;
	unsigned int errCnt = RADIO_CTS_TIMEOUT;

	while (errCnt != 0)      //wait until radio IC is ready with the data
	{
		Si4455_Hal.ClearNsel();
		Si4455_Hal.SpiWriteByte(0x44);    //read CMD buffer
		ctsVal = Si4455_Hal.SpiReadByte();

		if (ctsVal == 0xFF)
		{
			if (byteCount)
			{
				Si4455_Hal.SpiReadData(byteCount, pData);
			}

			Si4455_Hal.SetNsel();
			break;
		}

		Si4455_Hal.SetNsel();
		errCnt--;
	}

	if (errCnt == 0)
	{
		while (1)
		{
			/* ERROR!!!!  CTS should never take this long. */
#ifdef RADIO_COMM_ERROR_CALLBACK
			RADIO_COMM_ERROR_CALLBACK();
#endif
		}
	}

	if (ctsVal == 0xFF)
	{
		ctsWentHigh = 1;
	}

	return ctsVal;
}

/*!
* Sends a command to the radio chip
*
* @param byteCount     Number of bytes in the command to send to the radio device
* @param pData         Pointer to the command to send.
*/
void tSi4455_RadioComm::SendCmd(byte byteCount, byte* pData)
{
	while (!ctsWentHigh)
	{
		PollCTS();
	}

	Si4455_Hal.ClearNsel();
	Si4455_Hal.SpiWriteData(byteCount, pData);
	Si4455_Hal.SetNsel();
	ctsWentHigh = 0;
}

/*!
* Gets a command response from the radio chip
*
* @param cmd           Command ID
* @param pollCts       Set to poll CTS
* @param byteCount     Number of bytes to get from the radio chip.
* @param pData         Pointer to where to put the data.
*/
void tSi4455_RadioComm::ReadData(byte cmd, boolean pollCts, byte byteCount, byte* pData)
{
	if (pollCts)
	{
		while (!ctsWentHigh)
		{
			PollCTS();
		}
	}

	Si4455_Hal.ClearNsel();
	Si4455_Hal.SpiWriteByte(cmd);
	Si4455_Hal.SpiReadData(byteCount, pData);
	Si4455_Hal.SetNsel();
	ctsWentHigh = 0;
}


/*!
* Gets a command response from the radio chip
*
* @param cmd           Command ID
* @param pollCts       Set to poll CTS
* @param byteCount     Number of bytes to get from the radio chip
* @param pData         Pointer to where to put the data
*/
void tSi4455_RadioComm::WriteData(byte cmd, boolean pollCts, byte byteCount, const byte* pData)
{
	if (pollCts)
	{
		while (!ctsWentHigh)
		{
			PollCTS();
		}
	}

	Si4455_Hal.ClearNsel();
	Si4455_Hal.SpiWriteByte(cmd);
	Si4455_Hal.SpiWriteData(byteCount, pData);
	Si4455_Hal.SetNsel();
	ctsWentHigh = 0;
}

/*!
* Waits for CTS to be high
*
* @return CTS value
*/
byte tSi4455_RadioComm::PollCTS()
{
#ifdef RADIO_USER_CFG_USE_GPIO1_FOR_CTS
	while (!Si4455_Hal.Gpio1Level())
	{
		/* Wait...*/
	}

	ctsWentHigh = true;
	return 0xFF;
#else
	return GetResp(0, 0);
#endif
}

/**
* Clears the CTS state variable.
*/
void tSi4455_RadioComm::ClearCTS()
{
	ctsWentHigh = false;
}

/*!
* Sends a command to the radio chip and gets a response
*
* @param cmdByteCount  Number of bytes in the command to send to the radio device
* @param pCmdData      Pointer to the command data
* @param respByteCount Number of bytes in the response to fetch
* @param pRespData     Pointer to where to put the response data
*
* @return CTS value
*/
byte tSi4455_RadioComm::SendCmdGetResp(byte cmdByteCount, byte* pCmdData, byte respByteCount, byte* pRespData)
{
	SendCmd(cmdByteCount, pCmdData);
	return GetResp(respByteCount, pRespData);
}


/*!
* File:
*  radio_comm.h
*
* Description:
*  This file contains the RADIO communication layer.
*
* Silicon Laboratories Confidential
* Copyright 2011 Silicon Laboratories, Inc.
*/
#ifndef _RADIO_COMM_H_
#define _RADIO_COMM_H_

#define RADIO_CTS_TIMEOUT 10000

class tSi4455_RadioComm
{	
public:
	byte GetResp(byte byteCount, byte* pData);
	void SendCmd(byte byteCount, byte* pData);
	void ReadData(byte cmd, boolean pollCts, byte byteCount, byte* pData);
	void WriteData(byte cmd, boolean pollCts, byte byteCount, const byte* pData);
	
	byte PollCTS();
	byte SendCmdGetResp(byte cmdByteCount, byte* pCmdData, byte respByteCount, byte* pRespData);
	void ClearCTS();
};

extern tSi4455_RadioComm Si4455_RadioComm;
	
#endif //_RADIO_COMM_H_

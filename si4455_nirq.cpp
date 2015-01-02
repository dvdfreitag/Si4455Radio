/*
* Silicon Laboratories Confidential
* Copyright 2011 Silicon Laboratories, Inc.
*/

#include "si4455_nirq.h"
#include "si4455_api_lib.h"

#define Si4455ChipReadyPend                     (Si4455Cmd.GET_INT_STATUS.PH_PEND << 2) & 0x01
#define Si4455CmdErrPend                        (Si4455Cmd.GET_INT_STATUS.PH_PEND << 3) & 0x01
#define Si4455StateChangePend                   (Si4455Cmd.GET_INT_STATUS.PH_PEND << 4) & 0x01
#define Si4455FifoUnderflowOverflowErrorPend    (Si4455Cmd.GET_INT_STATUS.PH_PEND << 5) & 0x01
#define Si4455CalPend                           (Si4455Cmd.GET_INT_STATUS.PH_PEND << 6) & 0x01

#define Si4455SyncDetectPend                    Si4455Cmd.GET_INT_STATUS.MODEM_PEND & 0x01
#define Si4455PreambleDetectPend                (Si4455Cmd.GET_INT_STATUS.MODEM_PEND << 1) & 0x01
#define Si4455InvalidPreamblePend               (Si4455Cmd.GET_INT_STATUS.MODEM_PEND << 2) & 0x01
#define Si4455RssiPend                          (Si4455Cmd.GET_INT_STATUS.MODEM_PEND << 3) & 0x01
#define Si4455RssiJumpPend                      (Si4455Cmd.GET_INT_STATUS.MODEM_PEND << 4) & 0x01
#define Si4455InvalidSyncPend                   (Si4455Cmd.GET_INT_STATUS.MODEM_PEND << 5) & 0x01
#define Si4455PostambleDetectPend               (Si4455Cmd.GET_INT_STATUS.MODEM_PEND << 6) & 0x01

#define Si4455RxFifoAlmostFullPend              Si4455Cmd.GET_INT_STATUS.CHIP_PEND & 0x01
#define Si4455TxFifoAlmostEmptyPend             (Si4455Cmd.GET_INT_STATUS.CHIP_PEND << 1) & 0x01
#define Si4455Crc32ErrorPend                    (Si4455Cmd.GET_INT_STATUS.CHIP_PEND << 3) & 0x01
#define Si4455PacketRxPend                      (Si4455Cmd.GET_INT_STATUS.CHIP_PEND << 4) & 0x01
#define Si4455PacketSentPend                    (Si4455Cmd.GET_INT_STATUS.CHIP_PEND << 5) & 0x01

tSi4455_nIRQ Si4455_nIRQ = tSi4455_nIRQ();

/*!
* This function is used to handle the assertion of nIRQ of the si4455 chip.
*
* @note This function can take up to 6 ms depending on the startup time of the
* si4455 chip.
*
*/
void tSi4455_nIRQ::ProcessIRQ()
{
#ifdef SI4455_USER_CONFIG_USE_FRR_ABC_FOR_NIRQ
	byte pendBits[3];

	si4455_frr_read();
#else
	Si4455_API.GetIntStatus(0, 0, 0);
#endif
}

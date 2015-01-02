/*!
* File:
*  si4455_api_lib.c
*
* Description:
*  This file contains the Si4455 API library.
*
* Silicon Laboratories Confidential
* Copyright 2011 Silicon Laboratories, Inc.
*/

#include <stdarg.h>
#include "si4455_api_lib.h"
#include "radio_hal.h"
#include "radio_comm.h"
#include "si4455_defs.h"

#define SI4455_USER_CONFIG_XTAL_FREQ 30000000L

union si4455_cmd_reply_union Si4455Cmd;
byte radioCmd[16];

tSi4455_API Si4455_API = tSi4455_API();

/*!
* This functions is used to reset the si4455 radio by applying shutdown and
* releasing it.  After this function @ref si4455_boot should be called.  You
* can check if POR has completed by waiting 4 ms or by polling GPIO 0, 2, or 3.
* When these GPIOs are high, it is safe to call @ref si4455_boot.
*/
void tSi4455_API::Reset()
{
	byte loopCount;

	/* Put radio in shutdown, wait then release */
	Si4455_Hal.AssertShutdown();
	//! @todo this needs to be a better delay function.
	for (loopCount = 255; loopCount != 0; loopCount--);
	Si4455_Hal.DeassertShutdown();
	for (loopCount = 255; loopCount != 0; loopCount--);
	Si4455_RadioComm.ClearCTS();
}

void tSi4455_API::PowerUp(byte BOOT_OPTIONS, byte XTAL_OPTIONS, unsigned long XO_FREQ)
{
	radioCmd[0] = SI4455_CMD_ID_POWER_UP;
	radioCmd[1] = BOOT_OPTIONS;
	radioCmd[2] = XTAL_OPTIONS;
	radioCmd[3] = (byte)(XO_FREQ >> 24);
	radioCmd[4] = (byte)(XO_FREQ >> 16);
	radioCmd[5] = (byte)(XO_FREQ >> 8);
	radioCmd[6] = (byte)(XO_FREQ);

	Si4455_RadioComm.SendCmd(SI4455_CMD_ARG_COUNT_POWER_UP, radioCmd);
}

/*!
* This function is used to load all properties and commands with a list of NULL terminated commands.
* Before this function @si446x_reset should be called.
*/
byte tSi4455_API::Initialize(const byte* pSetPropCmd)
{
	byte col;
	byte response;
	byte numOfBytes;

	/* While cycle as far as the pointer points to a command */
	while (*pSetPropCmd != 0x00)
	{
		/* Commands structure in the array:
		* --------------------------------
		* LEN | <LEN length of data>
		*/

		numOfBytes = *pSetPropCmd++;

		if (numOfBytes > 16u)
		{
			/* Initial configuration of Si4x55 */
			if (SI4455_CMD_ID_WRITE_TX_FIFO == *pSetPropCmd) 
			{
				if (numOfBytes > 128u)
				{
					/* Number of command bytes exceeds maximal allowable length */
					return SI4455_COMMAND_ERROR;
				}

				/* Load array to the device */
				pSetPropCmd++;
				WriteEZConfigArray(numOfBytes - 1, pSetPropCmd);

				/* Point to the next command */
				pSetPropCmd += numOfBytes - 1;

				/* Continue command interpreter */
				continue;
			}
			else
			{
				/* Number of command bytes exceeds maximal allowable length */
				return SI4455_COMMAND_ERROR;
			}
		}

		for (col = 0u; col < numOfBytes; col++)
		{
			radioCmd[col] = *pSetPropCmd;
			pSetPropCmd++;
		}

		if (Si4455_RadioComm.SendCmdGetResp(numOfBytes, radioCmd, 1, &response) != 0xFF)
		{
			/* Timeout occured */
			return SI4455_CTS_TIMEOUT;
		}

		/* Check response byte of EZCONFIG_CHECK command */
		if (SI4455_CMD_ID_EZCONFIG_CHECK == radioCmd[0]) 
		{
			if (response)
			{
				/* Number of command bytes exceeds maximal allowable length */
				return SI4455_COMMAND_ERROR;
			}    
		}

		if (Si4455_Hal.NirqLevel() == 0)
		{
			/* Get and clear all interrupts.  An error has occured... */
			GetIntStatus(0, 0, 0);
			if (Si4455Cmd.GET_INT_STATUS.CHIP_PEND & SI4455_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
			{
				return SI4455_COMMAND_ERROR;
			}
		}
	}

	return SI4455_SUCCESS;
}

void tSi4455_API::WriteEZConfigArray(byte numBytes, const byte* pEzConfigArray)
{
	Si4455_RadioComm.WriteData(SI4455_CMD_ID_WRITE_TX_FIFO, 1, numBytes, pEzConfigArray);
}

void tSi4455_API::EZConfigCheck(unsigned int CHECKSUM)
{
	/* Do not check CTS */

	Si4455_Hal.ClearNsel();

	/* Command byte */
	Si4455_Hal.SpiWriteByte(SI4455_CMD_ID_EZCONFIG_CHECK);

	/* CRC */
	Si4455_Hal.SpiWriteByte((unsigned int) CHECKSUM >> 8u);
	Si4455_Hal.SpiWriteByte((unsigned int) CHECKSUM & 0x00FF);

	Si4455_Hal.SetNsel();

	/* Get the respoonse from the radio chip */
	Si4455_RadioComm.GetResp(1u, radioCmd);
}

void tSi4455_API::StartTX(byte CHANNEL, byte CONDITION, unsigned int TX_LEN)
{
	radioCmd[0] = SI4455_CMD_ID_START_TX;
	radioCmd[1] = CHANNEL;
	radioCmd[2] = CONDITION;
	radioCmd[3] = (byte)(TX_LEN >> 8);
	radioCmd[4] = (byte)(TX_LEN);

	Si4455_RadioComm.SendCmd(SI4455_CMD_ARG_COUNT_START_TX, radioCmd);
}

void tSi4455_API::StartRX(byte CHANNEL, byte CONDITION, unsigned int RX_LEN, byte NEXT_STATE1, byte NEXT_STATE2, byte NEXT_STATE3)
{
	radioCmd[0] = SI4455_CMD_ID_START_RX;
	radioCmd[1] = CHANNEL;
	radioCmd[2] = CONDITION;
	radioCmd[3] = (byte)(RX_LEN >> 8);
	radioCmd[4] = (byte)(RX_LEN);
	radioCmd[5] = NEXT_STATE1;
	radioCmd[6] = NEXT_STATE2;
	radioCmd[7] = NEXT_STATE3;

	Si4455_RadioComm.SendCmd(SI4455_CMD_ARG_COUNT_START_RX, radioCmd);
}

void tSi4455_API::GetIntStatus(byte PH_CLR_PEND, byte MODEM_CLR_PEND, byte CHIP_CLR_PEND)
{
	radioCmd[0] = SI4455_CMD_ID_GET_INT_STATUS;
	radioCmd[1] = PH_CLR_PEND;
	radioCmd[2] = MODEM_CLR_PEND;
	radioCmd[3] = CHIP_CLR_PEND;

	Si4455_RadioComm.SendCmdGetResp(SI4455_CMD_ARG_COUNT_GET_INT_STATUS,
		radioCmd,
		SI4455_CMD_REPLY_COUNT_GET_INT_STATUS,
		radioCmd);

	Si4455Cmd.GET_INT_STATUS.INT_PEND       = radioCmd[0];
	Si4455Cmd.GET_INT_STATUS.INT_STATUS     = radioCmd[1];
	Si4455Cmd.GET_INT_STATUS.PH_PEND        = radioCmd[2];
	Si4455Cmd.GET_INT_STATUS.PH_STATUS      = radioCmd[3];
	Si4455Cmd.GET_INT_STATUS.MODEM_PEND     = radioCmd[4];
	Si4455Cmd.GET_INT_STATUS.MODEM_STATUS   = radioCmd[5];
	Si4455Cmd.GET_INT_STATUS.CHIP_PEND      = radioCmd[6];
	Si4455Cmd.GET_INT_STATUS.CHIP_STATUS    = radioCmd[7];
}

void tSi4455_API::GpioPinCfg(byte GPIO0, byte GPIO1, byte GPIO2, byte GPIO3, byte NIRQ, byte SDO, byte GEN_CONFIG)
{
	radioCmd[0] = SI4455_CMD_ID_GPIO_PIN_CFG;
	radioCmd[1] = GPIO0;
	radioCmd[2] = GPIO1;
	radioCmd[3] = GPIO2;
	radioCmd[4] = GPIO3;
	radioCmd[5] = NIRQ;
	radioCmd[6] = SDO;
	radioCmd[7] = GEN_CONFIG;

	Si4455_RadioComm.SendCmdGetResp( SI4455_CMD_ARG_COUNT_GPIO_PIN_CFG,
		radioCmd,
		SI4455_CMD_REPLY_COUNT_GPIO_PIN_CFG,
		radioCmd );

	Si4455Cmd.GPIO_PIN_CFG.GPIO0        = radioCmd[0];
	Si4455Cmd.GPIO_PIN_CFG.GPIO1        = radioCmd[1];
	Si4455Cmd.GPIO_PIN_CFG.GPIO2        = radioCmd[2];
	Si4455Cmd.GPIO_PIN_CFG.GPIO3        = radioCmd[3];
	Si4455Cmd.GPIO_PIN_CFG.NIRQ         = radioCmd[4];
	Si4455Cmd.GPIO_PIN_CFG.SDO          = radioCmd[5];
	Si4455Cmd.GPIO_PIN_CFG.GEN_CONFIG   = radioCmd[6];
}

void tSi4455_API::SetProperty(byte GROUP, byte NUM_PROPS, byte START_PROP, ...)
{
	va_list argList;
	byte cmdIndex;

	radioCmd[0] = SI4455_CMD_ID_SET_PROPERTY;
	radioCmd[1] = GROUP;
	radioCmd[2] = NUM_PROPS;
	radioCmd[3] = START_PROP;

	va_start (argList, START_PROP);
	cmdIndex = 4;
	while (NUM_PROPS--)
	{
		radioCmd[cmdIndex] = (byte)(va_arg (argList, int));
		cmdIndex++;
	}
	va_end(argList);

	Si4455_RadioComm.SendCmd(cmdIndex, radioCmd);
}

void tSi4455_API::ChangeState(byte NEXT_STATE1)
{
	radioCmd[0] = SI4455_CMD_ID_CHANGE_STATE;
	radioCmd[1] = NEXT_STATE1;

	Si4455_RadioComm.SendCmd(SI4455_CMD_ARG_COUNT_CHANGE_STATE, radioCmd);
}

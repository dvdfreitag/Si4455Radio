/*! @file radio.h
* @brief This file contains the public functions controlling the radio chip.
*
* @b COPYRIGHT
* @n Silicon Laboratories Confidential
* @n Copyright 2012 Silicon Laboratories, Inc.
* @n http://www.silabs.com
*/

#ifndef RADIO_H_
#define RADIO_H_

typedef struct
{
	const byte *Radio_ConfigurationArray;

	byte Radio_ChannelNumber;
	byte Radio_PacketLength;
	byte Radio_State_After_Power_Up;

	unsigned int Radio_Delay_Cnt_After_Reset;
} tRadioConfiguration;

class tRadio
{	
	public:
		void Initialize();
		void StartTX(byte, byte *);
		void StartRX(byte);
		byte CheckEZConfig(unsigned int);
		
	private:
		void PowerUp();
};

extern tRadio Radio;

#endif /* RADIO_H_ */

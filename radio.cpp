/*! @file radio.c
 * @brief This file contains functions to interface with the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include <Arduino.h>
#include "si4455_api_lib.h"
#include "radio.h"
#include "radio_config.h"

const byte Radio_Configuration_Data_Array[] = RADIO_CONFIGURATION_DATA_ARRAY;
const tRadioConfiguration RadioConfiguration = RADIO_CONFIGURATION_DATA;

tRadio Radio = tRadio();

/*!
 *  Power up the Radio.
 *
 *  @note
 *
 */
void tRadio::PowerUp()
{
  /* Hardware reset the chip */
  Si4455_API.Reset();

  /* Wait until reset timeout or Reset IT signal */
  for (unsigned int wDelay = 0; wDelay < RadioConfiguration.Radio_Delay_Cnt_After_Reset; wDelay++);
}

/*!
 *  Radio Initialization.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 */
void tRadio::Initialize()
{
  /* Power Up the radio chip */
  PowerUp();

  /* Load radio configuration */
  while (SI4455_SUCCESS != Si4455_API.Initialize(RadioConfiguration.Radio_ConfigurationArray))
  {
    /* Error hook */
    //LED4 = !LED4;

    for (unsigned int wDelay = 0x7FFF; wDelay--;);

    /* Power Up the radio chip */
    PowerUp();
  }

  // Read ITs, clear pending ones
  Si4455_API.GetIntStatus(0, 0, 0);
}

/*!
 *  Set Radio to TX mode, fixed packet length.
 *
 *  @param channel Freq. Channel, Packet to be sent
 *
 *  @note
 *
 */
void tRadio::StartTX(byte channel, byte *pioFixRadioPacket)
{
  // Read ITs, clear pending ones
  Si4455_API.GetIntStatus(0, 0, 0);

  /* Start sending packet on channel, START immediately, Packet according to PH */
  Si4455_API.StartTX(channel, 0, 0);
}
/*!
 *  Set Radio to RX mode, fixed packet length.
 *
 *  @param channel Freq. Channel
 *
 *  @note
 *
 */
void tRadio::StartRX(byte channel)
{
  // Read ITs, clear pending ones
  Si4455_API.GetIntStatus(0, 0, 0);

  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
  Si4455_API.StartRX(channel, 0, RadioConfiguration.Radio_PacketLength,
                  SI4455_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_RX,
                  SI4455_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
                  SI4455_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
}


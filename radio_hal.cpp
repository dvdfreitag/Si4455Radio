/*!
* File:
*  radio_hal.c
*
* Description:
*  This file contains RADIO HAL.
*
* Silicon Laboratories Confidential
* Copyright 2011 Silicon Laboratories, Inc.
*/

#include <Arduino.h>
#include <SPI.h>
#include "radio_hal.h"

tSi4455_Hal Si4455_Hal = tSi4455_Hal();

// This function sets the SDN pin on the Si4455 high to shut the device down. (contents of all registers are lost.)
void tSi4455_Hal::AssertShutdown()
{
	digitalWrite(SDNPin, HIGH);
}

// This function sets the SDN pin on the Si4455 low to enable the device.
void tSi4455_Hal::DeassertShutdown()
{
	digitalWrite(SDNPin, LOW);
}

// This function sets the nSEL pin on the Si4455 low to either select or disable the SPI interface.
void tSi4455_Hal::ClearNsel()
{
	digitalWrite(nSELPin, LOW);
}

// This function sets the nSEL pin on the Si4455 high to either select or enable the SPI interface.
void tSi4455_Hal::SetNsel()
{
	digitalWrite(nSELPin, HIGH);
}

// This function reads the nIRQ pin on the Si4455 device to determine if an interrupt has occcurred.
boolean tSi4455_Hal::NirqLevel()
{
	return digitalRead(nIRQPin);
}

// This function writes a byte to the Si4455 device using SPI
void tSi4455_Hal::SpiWriteByte(byte byteToWrite)
{
	SPI.transfer(byteToWrite);
}

// This function reads a byte from the Si4455 device using SPI
byte tSi4455_Hal::SpiReadByte()
{
	return SPI.transfer(0xFF);
}

// This function writes a buffer to the Si4455 using SPI
void tSi4455_Hal::SpiWriteData(byte byteCount, const byte* pData)
{
	const byte* ptr = pData;

	for (int i = 0; i < byteCount; i++)
	{
		SPI.transfer(*ptr++);
	}
}

// This function reads a buffer from the Si4455 using SPI
void tSi4455_Hal::SpiReadData(byte byteCount, byte* pData)
{
	byte* ptr = pData;

	for (int i = 0; i < byteCount; i++)
	{
		*ptr++ = SPI.transfer(0xFF);
	}
}

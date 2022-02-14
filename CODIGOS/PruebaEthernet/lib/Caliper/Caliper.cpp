/*
*	Caliper.cpp, Copyright Jonathan Mackey 2018
*	Class to control the I2C caliper interface CaliperI2CTiny85/84A.
*	See CaliperI2CTest.ino for a usage example.
*
*	GNU license:
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Please maintain this license information along with authorship and copyright
*	notices in any redistribution of this code.
*
*/
#include "Arduino.h"
#include "Caliper.h"
#include <Wire.h>

volatile bool	gDataChanged;

/*********************************** Caliper ************************************/
Caliper::Caliper(
	uint32_t	inAddress,
	uint8_t		inChangePin)	// Must be a valid interrupt pin
	: mAddress(inAddress), mChangePin(inChangePin)
{
}

/*********************************** begin ************************************/
void Caliper::begin(void)
{
	pinMode(mChangePin, INPUT);  // Data Changed pin
	attachInterrupt(digitalPinToInterrupt(mChangePin), Caliper::DataChangedISR, RISING);
	gDataChanged = true;
	
	Wire.begin();
}

/****************************** DataChangedISR ********************************/
void Caliper::DataChangedISR(void)
{
	gDataChanged = true;
}

/******************************** DataChanged *********************************/
bool Caliper::DataChanged(void) const
{
	return(gDataChanged);
}

/***************************** ClearDataChanged *******************************/
void Caliper::ClearDataChanged(void)
{
	gDataChanged = false;
}

/********************************** ReadRaw ***********************************/
uint32_t Caliper::ReadRaw(void)
{
	gDataChanged = false;
	uint8_t	bytesRecieved = Wire.requestFrom(mAddress, (uint8_t)3, (uint8_t)true);
	if (bytesRecieved == 3)
	{
		for (uint8_t i = 3; i != 0; i--)
		{
			mRawValue <<= 8;
			mRawValue |= (uint8_t)Wire.read();
		}
	} else
	{
		mRawValue = 0xFFFFFFFF;	// Mark value as invalid
	}
}

/********************************* ReadValue **********************************/
float Caliper::ReadValue(void)
{
	ReadRaw();
	return(GetValue());
}

/*********************************** Sleep ************************************/
void Caliper::Sleep(void) const
{
	Wire.beginTransmission(mAddress);
	Wire.write('0');
	Wire.endTransmission(true);
}

/*********************************** Wake ************************************/
bool Caliper::Wake(void) const
{
	Wire.beginTransmission(mAddress);
	Wire.write('1');
	Wire.endTransmission(true);
}

/*********************************** Zero ************************************/
bool Caliper::Zero(void) const
{
	Sleep();
	delay(300);
	Wake();
}


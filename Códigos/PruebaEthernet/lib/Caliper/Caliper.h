/*
*	Caliper.h, Copyright Jonathan Mackey 2018
*	Class to control the I2C caliper interface CaliperI2CTiny85.
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
#ifndef Caliper_H
#define Caliper_H
#include "CaliperUtils.h"

class Caliper
{
public:
							Caliper(
								uint32_t				inAddress,
								uint8_t					inChangePin);
	void					begin(void);
	void					Sleep(void) const;
	bool					Wake(void) const;
	bool					Zero(void) const;
	uint32_t				ReadRaw(void);
	uint32_t				RawValue(void) const
								{return(mRawValue);}
	float					ReadValue(void);
	float					GetValue(
								uint8_t*				outDecimalPlaces = NULL) const
								{return(CaliperUtils::RawToValue(mRawValue, outDecimalPlaces));}
	bool					IsMetric(void) const
								{return(CaliperUtils::IsMetric(mRawValue));}
	bool					ValueIsValid(void) const
								{return(CaliperUtils::ValueIsValid(mRawValue));}
	bool					DataChanged(void) const;
	void					ClearDataChanged(void);
protected:
	uint8_t		mAddress;		// Device address
	uint8_t		mChangePin;
	uint32_t	mRawValue;
	
    static void				DataChangedISR(void);
};

#endif
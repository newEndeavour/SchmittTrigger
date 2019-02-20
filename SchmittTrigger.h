/*
  File:         SchmittTrigger.h
  Version:      0.0.3
  Date:         05-Jan-2019
  Revision:     20-Feb-2019
  Author:       Jerome Drouin (jerome.p.drouin@gmail.com)

  SchmittTrigger.h - Library for 'duino / Wiring
  https://github.com/newEndeavour/SchmittTrigger
  http://playground.arduino.cc/Main/SchmittTrigger

  SchmittTrigger implements a Nysteresis dual threshold action. This
  particular kind of trigger is useful for determining digital states in
  noisy environments. 

  Copyright (c) 2018-2019 Jerome Drouin  All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Editions:
  - 0.0.1	: First version
  - 0.0.2	: Introducing hierarchy in error coding returns
  - 0.0.3	: Introducing Ref_Level & Threshold Factors so we can update 
		  Thresholds from initially set Factors
		  Call SetReferenceLevel(Reflevel,0) to keep Factors constant
		  call SetReferenceLevel(Reflevel,1) to recalculate Factors
		  

*/


// ensure this library description is only included once
#ifndef SchmittTrigger_h
#define SchmittTrigger_h


#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define MAX_DEBOUNCE	20	// MAX DEBOUNCE PARAM. 


// library interface description
class SchmittTrigger
{
  // user-accessible "public" interface
  public:
  // methods
	SchmittTrigger(float _Ref_Level, float _Press_Thres, float _Release_Thres, uint8_t _Press_Debounce, uint8_t _Release_Debounce, int _Operation);
	void 	resetTriggerStatus(void);
	void 	resetTriggerParameters(void);

	int 	updateStatus(float lastObs);
	int 	GetStatus(void);

	void 	SetOperation(int _Operation);
	int 	GetOperation();

	void 	SetReferenceLevel(float _Ref_Level, int recalc_factors);
	float 	GetReferenceLevel(void);

	void 	SetPressThreshold(float _Press_Thres);
	void 	SetReleaseThreshold(float _Release_Thres);	
	float 	GetPressThreshold(void);
	float 	GetReleaseThreshold(void);

	float 	GetPressThresholdFactor(void);
	float 	GetReleaseThresholdFactor(void);
	void 	SetPressThresholdFactor(float _Press_Thres_Factor);
	void 	SetReleaseThresholdFactor(float _Release_Thres_Factor);	

	void 	SetPressDebounce(uint8_t _Press_Debounce);
	void 	SetReleaseDebounce(uint8_t _Release_Debounce);

	uint8_t GetPressCount(void);
	uint8_t GetReleaseCount(void);

	uint8_t GetPressDebounce(void);
	uint8_t GetReleaseDebounce(void);


  // library-accessible "private" interface
  private:
  // variables
	int 	error;
	int 	Operation;		// =0: Rising only; =1: Falling only.

	uint8_t	Status;			// Current Trigger value: either 0 or 1

	float	Ref_Level;		// Reference Level
	float	Press_Thres;		// Thresold Value to activate Press Counter
	float	Release_Thres;		// Thresold Value to activate Release Counter

	float	Press_Thres_Factor;	// Thresold Factor (vs Ref_Level) to activate Press Counter
	float	Release_Thres_Factor;	// Thresold Factor (vs Ref_Level) to activate Release Counter

	uint8_t Press_Debounce;		// number of consecutive counts to change CurrentStatus from 0 to 1
					// if this parameter is too large, the trigger might never be = 1
					// if too small (=1), the Schmitt Trigger might be very noisy 
	uint8_t Release_Debounce;	// number of consecutive counts to change CurrentStatus from 1 to 0

	uint8_t	Press_Count;		// current number of times Press threshold is crossed (lastObs>=Press_Count) consecutively
					// "une hirondelle ne fait pas le primtemps..."
	uint8_t	Release_Count;		// current number of times Release threshold is crossed (lastObs<Release_Count) consecutively
	
  // methods
	void 	ResetErrors(void);	// Error flag handling
	void 	Update_Threshold_Factors(void);		// Called when setting Levels
	void 	Update_Threshold_Levels(void);		// Called when setting Factors directly

};

#endif

/*
  File:         SchmittTrigger.cpp
  Version:      0.0.3
  Date:         05-Jan-2019
  Revision:     20-Feb-2019
  Author:       Jerome Drouin (jerome.p.drouin@gmail.com)

  Editions:	Please go to SchmittTrigger.h for Edition Notes.

  SchmittTrigger.cpp - Library for 'duino / Wiring
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

*/


#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif

#include "SchmittTrigger.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

SchmittTrigger::SchmittTrigger(float _Ref_Level, float _Press_Thres, float _Relea_Thres, uint8_t _Press_debounce, uint8_t _Relea_debounce, int _operation)
{
int op_fact = 0;


	// Object data
	Operation 		= _operation;		// =0: Rising only; =1: Falling only; =2: double (Rising & Falling)

	//Set initial values	
	Ref_Level		= _Ref_Level;		// Reference Level for Thresholds
	Press_Thres		= _Press_Thres;		// Thresold Value to activate Press Counter
	Relea_Thres		= _Relea_Thres;	// Thresold Value to activate Relea Counter
	Update_Threshold_Factors();

	Press_Debounce		= _Press_debounce;	// number of consecutive counts required to change Status from 0 to 1
	Relea_Debounce		= _Relea_debounce;	// number of consecutive counts required to change Status from 1 to 0

	resetTriggerStatus();				// Reset Trigger data

	// Object parameter's error handling
	ResetErrors();

}


// Public Methods //////////////////////////////////////////////////////////////
//Resets current Trigger to false
void SchmittTrigger::resetTriggerStatus(void) 
{

	Status			= 0;			// Current Trigger value: reset to 0 (false)
	Press_Count 		= 0;			// current number of times Press_threshold is crossed
	Relea_Count		= 0;			// current number of times Relea_threshold is crossed

	ResetErrors();					// Object parameter's error handling
}


//Resets current Trigger parameters
void SchmittTrigger::resetTriggerParameters(void) 
{

	Ref_Level		= -1;
	Press_Thres		= -1;
	Relea_Thres		= -1;

	resetTriggerStatus();				// Reset Trigger data

	ResetErrors();					// Object parameter's error handling
}


//Sets Operation mode
void SchmittTrigger::SetOperation(int _operation)
{

	//Object data
	Operation 		= _operation;		// =0: Rising only; =1: Falling only; =2: double (Rising & Falling)

	ResetErrors();					// Object parameter's error handling
}


//Get Operation mode
int SchmittTrigger::GetOperation(void)
{
	return Operation;
}


// Update the Status of this Schmitt Trigger with the Last available Read and 
// returns the current Status level
int SchmittTrigger::updateStatus(float lastRead)
{
	if (error < 0) return error;            // bad constructor variables

	//0==Rising
	if (Operation==0) {
		//if Status==0
		if (Status==0) {
    			if (lastRead>=Press_Thres) {
	    			Press_Count++;			

	      	    		if (Press_Count>=Press_Debounce) {
		      			Status = 1;
				}
	
		  	} else {
				//Reset count
				Press_Count = 0;
			}
		} else {
			if (lastRead<Relea_Thres) {
				Relea_Count++;				

	      	    		if (Relea_Count>=Relea_Debounce) {
		      			Status = 0;
				}
		  	} else {
				//Reset count
				Relea_Count = 0;
			}
		}
	}

	//1==Falling
	if (Operation==1) {
		//if Status==0
		if (Status==0) {
    			if (lastRead<=Press_Thres) {
	    			Press_Count++;			

	      	    		if (Press_Count>=Press_Debounce) {
		      			Status = 1;
				}
	
		  	} else {
				//Reset count
				Press_Count = 0;
			}
		} else {
			if (lastRead>Relea_Thres) {
				Relea_Count++;				

	      	    		if (Relea_Count>=Relea_Debounce) {
		      			Status = 0;
				}
		  	} else {
				//Reset count
				Relea_Count = 0;
			}
		}
	}

	//2==Double
	if (Operation==2) {
		

		//NOT IMPLEMENTED JUST YET
		//(this requires the doubling or Press/Relea Thres parameters or a mirroring methodology

	}

	// Return   
	return Status;	
}


int SchmittTrigger::GetStatus(void)
{
	return Status;
}


void SchmittTrigger::SetReferenceLevel(float _Ref_Level, int recalc_factors)
{
	// Object data
	Ref_Level		= _Ref_Level;
	
	if (recalc_factors) 
		Update_Threshold_Factors();
	else
		Update_Threshold_Levels();

	// Object parameter's error handling
	ResetErrors();
}

void SchmittTrigger::SetPressThreshold(float _Press_Thres)
{
	// Object data
	Press_Thres		= _Press_Thres;

	Update_Threshold_Factors();

	// Object parameter's error handling
	ResetErrors();
}


void SchmittTrigger::SetReleaThreshold(float _Relea_Thres)
{
	// Object data
	Relea_Thres		= _Relea_Thres;

	Update_Threshold_Factors();

	// Object parameter's error handling
	ResetErrors();
}


void SchmittTrigger::SetPressThresholdFactor(float _Press_Thres_Factor)
{
	// Object data
	Press_Thres_Factor	= _Press_Thres_Factor;

	Update_Threshold_Levels();

	// Object parameter's error handling
	ResetErrors();
}


void SchmittTrigger::SetReleaThresholdFactor(float _Relea_Thres_Factor)
{
	// Object data
	Relea_Thres_Factor	= _Relea_Thres_Factor;

	Update_Threshold_Levels();

	// Object parameter's error handling
	ResetErrors();
}


void SchmittTrigger::SetPressDebounce(uint8_t _Press_debounce)
{
	// Object data
	Press_Debounce		= _Press_debounce;

	// Object parameter's error handling
	ResetErrors();
}


void SchmittTrigger::SetReleaDebounce(uint8_t _Relea_debounce)
{

	// Object data
	Relea_Debounce	= _Relea_debounce;

	// Object parameter's error handling
	ResetErrors();
}


float SchmittTrigger::GetReferenceLevel(void)
{
	return Ref_Level;
}


float SchmittTrigger::GetPressThreshold(void)
{
	return Press_Thres;
}


float SchmittTrigger::GetReleaThreshold(void)
{
	return Relea_Thres;
}


float SchmittTrigger::GetPressThresholdFactor(void)
{
	return Press_Thres_Factor;
}


float SchmittTrigger::GetReleaThresholdFactor(void)
{
	return Relea_Thres_Factor;
}


uint8_t SchmittTrigger::GetPressCount(void) 
{
	return Press_Count;
}


uint8_t SchmittTrigger::GetReleaCount(void) 
{
	return Relea_Count;
}


uint8_t SchmittTrigger::GetPressDebounce(void)
{
	return Press_Debounce;
}


uint8_t SchmittTrigger::GetReleaDebounce(void)
{
	return Relea_Debounce;
}


// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

//Reset error flag following importance hierarchy
//by increasing error importance:
//	- Operation	: -5
//	- Debounce	: -4
//	- Ref Level	: -3
//	- Thres position: -2
//	- Thres values	: -1	(most important)
//	Note: Object instanciated with incorrect Operation mode and incorrect threshold values
//	will return -1 (first), and then -4 only after theshold conflicts have been resolved.
//	
void SchmittTrigger::ResetErrors(void)
{
int op_fact = 0;

	// Object parameter's error handling
	error = 1;

	if ((Operation!=0) 
	 && (Operation!=1)
	 && (Operation!=2)) 			error =-5;	// incorrect Operation mode

	if (Press_Debounce<=0)	 		error =-4;	// incorrect Press_debounce variables
	if (Press_Debounce>MAX_DEBOUNCE) 	error =-4;	// incorrect Relea_debounce variables

	if (Relea_Debounce<=0) 		error =-4;	// incorrect Relea_debounce variables
	if (Relea_Debounce>MAX_DEBOUNCE) 	error =-4;	// incorrect Relea_debounce variables

	if (Ref_Level<0) 			error =-3;	// incorrect Ref_Level

	if (Operation==0) op_fact = 1; 
	if (Operation==1) op_fact = -1; 
	if ((Press_Thres*op_fact)	
		<(Relea_Thres*op_fact)) 	error =-2;	// incorrect thres relative placements

	if (Press_Thres<0) 			error =-1;	// incorrect Press_Thres
	if (Relea_Thres<0) 			error =-1;	// incorrect Relea_Thres

}


void SchmittTrigger::Update_Threshold_Factors(void)
{
	//Update Factors
	if (Ref_Level>0) {
		if (Press_Thres>0)
			Press_Thres_Factor = Press_Thres/Ref_Level;	
		if (Relea_Thres>0)
			Relea_Thres_Factor = Relea_Thres/Ref_Level;	
	} else {
		Press_Thres_Factor = 0;
		Relea_Thres_Factor = 0;
	}

}


void SchmittTrigger::Update_Threshold_Levels(void)
{
	//Update Thresholds Levels
	if (Ref_Level>0) {
		if (Press_Thres_Factor>0)
			Press_Thres = Press_Thres_Factor  * Ref_Level;	
		if (Relea_Thres>0)
			Relea_Thres = Relea_Thres_Factor * Ref_Level;	
	}

}


// /////////////////////////////////////////////////////////////////////////////


/*
  File:         SchmittTrigger.cpp
  Version:      0.0.1
  Date:         05-Jan-2019
  Revision:     20-Jan-2019
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

SchmittTrigger::SchmittTrigger(float _Press_Thres, float _Release_Thres, uint8_t _Press_debounce, uint8_t _Release_debounce, int _operation)
{
int op_fact = 0;


	// Object data
	Operation 		= _operation;		// =0: Rising only; =1: Falling only; =2: double (Rising & Falling)

	//Set initial values	
	Press_Thres		= _Press_Thres;		// Thresold Value to activate Press Counter
	Release_Thres		= _Release_Thres;		// Thresold Value to activate Release Counter

	Press_Debounce		= _Press_debounce;	// number of consecutive counts required to change Status from 0 to 1
	Release_Debounce	= _Release_debounce;	// number of consecutive counts required to change Status from 1 to 0

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
	Release_Count		= 0;			// current number of times Release_threshold is crossed

	ResetErrors();					// Object parameter's error handling
}


//Resets current Trigger parameters
void SchmittTrigger::resetTriggerParameters(void) 
{

	Press_Thres		= -1;
	Release_Thres		= -1;

	resetTriggerStatus();				// Reset Trigger data

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
			if (lastRead<Release_Thres) {
				Release_Count++;				

	      	    		if (Release_Count>=Release_Debounce) {
		      			Status = 0;
				}
		  	} else {
				//Reset count
				Release_Count = 0;
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
			if (lastRead>Release_Thres) {
				Release_Count++;				

	      	    		if (Release_Count>=Release_Debounce) {
		      			Status = 0;
				}
		  	} else {
				//Reset count
				Release_Count = 0;
			}
		}
	}

	//2==Double
	if (Operation==2) {
		

		//NOT IMPLEMENTED JUST YET
		//(this requires the doubling or Press/Release Thres parameters or a mirroring methodology

	}

	// Return   
	return Status;	
}


int SchmittTrigger::GetStatus(void)
{
	return Status;
}


void SchmittTrigger::SetPressThreshold(float _Press_Thres)
{
	// Object data
	Press_Thres		= _Press_Thres;

	// Object parameter's error handling
	ResetErrors();
}


void SchmittTrigger::SetReleaseThreshold(float _Release_Thres)
{
	// Object data
	Release_Thres		= _Release_Thres;

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


void SchmittTrigger::SetReleaseDebounce(uint8_t _Release_debounce)
{

	// Object data
	Release_Debounce	= _Release_debounce;

	// Object parameter's error handling
	ResetErrors();
}


float SchmittTrigger::GetPressThreshold(void)
{
	return Press_Thres;
}


float SchmittTrigger::GetReleaseThreshold(void)
{
	return Release_Thres;
}


uint8_t SchmittTrigger::GetPressCount(void) 
{
	return Press_Count;
}


uint8_t SchmittTrigger::GetReleaseCount(void) 
{
	return Release_Count;
}


uint8_t SchmittTrigger::GetPressDebounce(void)
{
	return Press_Debounce;
}


uint8_t SchmittTrigger::GetReleaseDebounce(void)
{
	return Release_Debounce;
}


// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

//Reset error flag
void SchmittTrigger::ResetErrors(void)
{
int op_fact = 0;

	// Object parameter's error handling
	error = 1;

	if (Press_Debounce<=0)	 		error =-1;	// incorrect _Press_debounce variables
	if (Press_Debounce>MAX_DEBOUNCE) 	error =-1;	// incorrect _Release_debounce variables

	if (Release_Debounce<=0) 		error =-1;	// incorrect _Release_debounce variables
	if (Release_Debounce>MAX_DEBOUNCE) 	error =-1;	// incorrect _Release_debounce variables

	if (Operation==0) op_fact = 1; 
	if (Operation==1) op_fact = -1; 
	if ((Press_Thres*op_fact)	
		<(Release_Thres*op_fact)) 	error =-2;	// incorrect thres relative placements

	if (Press_Thres<0) 			error =-3;	// incorrect Press_Thres
	if (Release_Thres<0) 			error =-3;	// incorrect Release_Thres

	if ((Operation!=0) 
	 && (Operation!=1)
	 && (Operation!=2)) 			error =-4;	// incorrect _operation mode

}




// /////////////////////////////////////////////////////////////////////////////


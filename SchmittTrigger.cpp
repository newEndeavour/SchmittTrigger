/*
  SchmittTrigger.h v.01 - Library for 'duino / Wiring
  https://github.com/JeromeDrouin/SchmittTrigger
  http://playground.arduino.cc/Main/SchmittTrigger
  Copyright (c) 2018 Jerome Drouin  All rights reserved.
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

SchmittTrigger::SchmittTrigger(float _highThres, float _lowThres, uint8_t _high_crossreq, uint8_t _low_crossreq)
{
	// initialize this instance's variables
	error = 1;
	if (_high_crossreq<=0) error =-1;		// incorrect High_Status_Thres variables
	if (_low_crossreq<=0) error =-1;		// incorrect Low_Status_Thres variables
	if (_highThres<_lowThres) error =-2;		// incorrect _highThres
	if (_highThres<0) error =-3;			// incorrect _highThres
	if (_lowThres<0) error =-3;			// incorrect _lowThres

	//Set initial values	
	CurrentStatus		= 0;			// Current Trigger value: either 0 or 1
	High_Thres		= _highThres;		// Thresold Value to activate HIGH Counter
	Low_Thres		= _lowThres;		// Thresold Value to activate LOW Counter

	High_Cross_Req		= _high_crossreq;	// number of consecutive counts required to change CurrentStatus from 0 to 1
	Low_Cross_Req		= _low_crossreq;	// number of consecutive counts required to change CurrentStatus from 1 to 0

	High_CrossCount 	= 0;			// current number of times high_threshold is crossed
	Low_CrossCount	 	= 0;			// current number of times low_threshold is crossed

}


// Public Methods //////////////////////////////////////////////////////////////
// Update the Status of this Schmitt Trigger with the Last available Read and 
// returns the current Status level
int SchmittTrigger::updateTriggerStatus(float lastRead)
{
	if (error < 0) return error;            // bad constructor variables

	//CurrentStatus==0
	if (CurrentStatus==0) {
    		if (lastRead>=High_Thres) {
	    		High_CrossCount++;			

      	    		if (High_CrossCount>=High_Cross_Req) {
	      			CurrentStatus = 1;
			}

	  	} else {
			//Reset count
			High_CrossCount = 0;
		}
	} else {
		if (lastRead<Low_Thres) {
			Low_CrossCount++;				

      	    		if (Low_CrossCount>=Low_Cross_Req) {
	      			CurrentStatus = 0;
			}
	  	} else {
			//Reset count
			Low_CrossCount = 0;
		}
	}
	// Return   
	return CurrentStatus;	
}


int SchmittTrigger::GetCurrentStatus(void)
{
	return CurrentStatus;
}


void SchmittTrigger::SetHighThreshold(float _highThres)
{
	if (_highThres<Low_Thres) error =-2;	// incorrect _highThres
	if (_highThres<0) error =-3;		// incorrect _highThres
	High_Thres	= _highThres;
}


void SchmittTrigger::SetLowThreshold(float _lowThres)
{
	error = 1;
	if (_lowThres<0) error =-3;		// incorrect _lowThres
	Low_Thres	= _lowThres;
}


void SchmittTrigger::SetHighCrossReq(uint8_t _high_crossReq)
{
	if (_high_crossReq<=0) error =-1;		// incorrect _high_crossReq variables
	High_Cross_Req	= _high_crossReq;
}


void SchmittTrigger::SetLowCrossReq(uint8_t _low_crossReq)
{
	if (_low_crossReq<=0) error =-1;		// incorrect _low_crossReq variables
	Low_Cross_Req	= _low_crossReq;
}


float SchmittTrigger::GetHighThreshold(void)
{
	return High_Thres;
}


float SchmittTrigger::GetLowThreshold(void)
{
	return Low_Thres;
}


uint8_t SchmittTrigger::GetHighCrossCount(void) 
{
	return High_CrossCount;
}

uint8_t SchmittTrigger::GetLowCrossCount(void) 
{
	return Low_CrossCount;
}


uint8_t SchmittTrigger::GetHighCrossReq(void)
{
	return High_Cross_Req;
}


uint8_t SchmittTrigger::GetLowCrossReq(void)
{
	return Low_Cross_Req;
}


// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library


// /////////////////////////////////////////////////////////////////////////////


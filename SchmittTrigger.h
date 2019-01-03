/*
  SchmittTrigger.h v.01 - Library for 'duino / Wiring
  https://github.com/JeromeDrouin/SchmittTrigger
  http://playground.arduino.cc/Main/SchmittTrigger
  Copyright (c) 2018 Jerome Drouin  All rights reserved.
*/


// ensure this library description is only included once
#ifndef SchmittTrigger_h
#define SchmittTrigger_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// library interface description
class SchmittTrigger
{
  // user-accessible "public" interface
  public:
  // methods
	SchmittTrigger(float _highThres, float _lowThres, uint8_t _high_crossreq, uint8_t _low_crossreq);

	int updateTriggerStatus(float lastRead);
	int GetCurrentStatus(void);

	void SetHighThreshold(float _highThres);
	void SetLowThreshold(float _lowThres);	

	void SetHighCrossReq(uint8_t _high_crossReq);
	void SetLowCrossReq(uint8_t _low_crossReq);

	float GetHighThreshold(void);
	float GetLowThreshold(void);

	uint8_t GetHighCrossCount(void);
	uint8_t GetLowCrossCount(void);

	uint8_t GetHighCrossReq(void);
	uint8_t GetLowCrossReq(void);


  // library-accessible "private" interface
  private:
  // variables
	int 	error;
	uint8_t	CurrentStatus;		// Current Trigger value: either 0 or 1

	float	High_Thres;		// Thresold Value to activate HIGH Counter
	float	Low_Thres;		// Thresold Value to activate LOW Counter

	uint8_t High_Cross_Req;	// number of consecutive counts to change CurrentStatus from 0 to 1
					// if this parameter is too high, the trigger might never be = 1
					// if too low (=1), the Schmitt Trigger might be very noisy 
	uint8_t Low_Cross_Req;		// number of consecutive counts to change CurrentStatus from 1 to 0

	uint8_t High_CrossCount;	// current number of times high threshold is crossed (lastRead>=High_CrossCount) consecutively
					// "une hirondelle ne fait pas le primtemps..."
	uint8_t Low_CrossCount;		// current number of times low threshold is crossed (lastRead<Low_CrossCount) consecutively
	
  // methods
};

#endif

/*
  File:         SchmittTriggerSketch.ino
  Version:      0.0.1
  Date:         19-Dec-2018
  Revision:     01-Feb-2019
  Author:       Jerome Drouin
  
  https://github.com/newEndeavour/SchmittTrigger

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
#include <SchmittTrigger.h>

//---- SchmittTrigger Object parameters ----------------------------------------------------
#define ST_RECALC_FACTORS               0     // Factors are not recalculated when Ref_Level is Updated (but Threshold Levels are)

#define ST1_PRESS_DEBOUNCE              1     // Debouncing Press Parameter
#define ST1_RELEASE_DEBOUNCE            3     // Debouncing Release Parameter
#define ST1_OPERATION                   0     // 0=Rising / 1=Falling / 2=Double  
float   ST1_INIT_REF_LEVEL       = 0.5;  
float   ST1_PRESS_THRES          = 0.6;       // Press Level
float   ST1_RELEA_THRES          = 0.4;       // Release Level
SchmittTrigger ST1(ST1_INIT_REF_LEVEL, ST1_PRESS_THRES, ST1_RELEA_THRES, ST1_PRESS_DEBOUNCE, ST1_RELEASE_DEBOUNCE, ST1_OPERATION);
int ST1_Status;

//---- General Variables ----------------------------------------------------
float RawSignal;
int LoopCount = 0;

//---- Setup ----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.print("\n---- Serial Started ----\n");

  //For info
  //Unnecessary as this is already supported in Constructor
  ST1.SetReferenceLevel(ST1_INIT_REF_LEVEL,ST_RECALC_FACTORS);
  ST1.SetPressThreshold(ST1_PRESS_THRES);
  ST1.SetReleaThreshold(ST1_RELEA_THRES);
  ST1.resetTriggerStatus();
  ST1_Status = ST1.GetStatus();

}

//---- Loop ----------------------------------------------------------------
void loop() {

  LoopCount++;
  
  //Generate Random Variable
  RawSignal     = (float) random(0,101)/100;
  
  //
  ST1_Status    = ST1.updateStatus(RawSignal);

  //-> Serial
  Serial.print("\n");
  Serial.print(LoopCount);
  Serial.print(")");
  Serial.print("\tRaw:");
  Serial.print(RawSignal,3);
  
  Serial.print("\tThreshold(");
  Serial.print(ST1_RELEA_THRES,2);
  Serial.print(";");
  Serial.print(ST1_PRESS_THRES,2);
  Serial.print(")");      
  
  Serial.print("\tdeBouncing{");
  Serial.print(ST1.GetReleaCount());
  Serial.print("|");
  Serial.print(ST1.GetPressCount());
  Serial.print("}");
  
  Serial.print("\tStatus:");
  Serial.print(ST1_Status);
  
  delay(1000);
}

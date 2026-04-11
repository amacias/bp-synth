/* DSP-Filter.h */
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INC_DSPFILTER_H_
#define INC_DSPFILTER_H_

//----------------------------------------------------

#include <stdint.h>
#include <math.h>
#include "CONSTANTS.h"


//----------------------------------------------------
#define FLTR_GAIN 					0.88f

//may tweak these a bit..
#define MAX_FREQ						2200.f//was 4000
#define MIN_FREQ						20.f//was 80

//----------------------------------------------------
typedef struct DSPfilter
{
	uint8_t 	type; // filter selection : LP, HP
	float 		freq;
	float 		gain;
	float 		q;
	float 		samp1;
	float 		samp2;
} DSPfilter;


//--Exported Functions-------------------------------------------------------------
void 	SetFilterValue(float val);
//------------------------------------------------------------------------------------
float calcFilterSample(float smpl);
//------------------------------------------------------------------------------------
void 	init_Filter(void);

void 	FilterFreq_set(uint8_t val);

void 	FilterRes_set(uint8_t val);

void 	FilterType_set(uint8_t val);

float procClip(float n);
/*-----------------------------------------------------------------------------------*/



#endif /* INC_DSPFILTER_H_ */

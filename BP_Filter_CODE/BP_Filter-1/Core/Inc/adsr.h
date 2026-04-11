/* adsr.h */
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

#ifndef INC_ADSR_H_
#define INC_ADSR_H_

/* Includes ------------------------------------------------------------------*/

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "CONSTANTS.h"
#include "math_tools.h"



//! Envelope states.
enum { ATTACK, DECAY, SUSTAIN, RELEASE, DONE };

typedef struct
{
	float 	attackRate_;
	float 	decayRate_;
	float 	sustainLevel_;
	float 	releaseRate_;
	float 	value_;
	int 	  state_;


} ADSR_t ;


/* Exported functions --------------------------------------------------------*/
void ADSR_init(ADSR_t *env);
void ADSR_keyOn(ADSR_t *env);
void ADSR_keyOff(ADSR_t *env);
void ADSR_setAttackTime(ADSR_t *env, float time);
void ADSR_setDecayTime(ADSR_t *env, float time);
void ADSR_setSustainLevel(ADSR_t *env, float level);
void ADSR_setReleaseTime(ADSR_t *env, float time);
float ADSR_computeSample( ADSR_t *env );


void AttTime_set(uint8_t val);
void DecTime_set(uint8_t val);
void SustLevel_set(uint8_t val);
void RelTime_set(uint8_t val);


#endif /* INC_ADSR_H_ */

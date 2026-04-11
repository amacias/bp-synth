/* oscillators.h */
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

#ifndef INC_OSCILLATORS_H_
#define INC_OSCILLATORS_H_
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "CONSTANTS.h"

/*-------------------------------------------------------*/
enum timbre { BASIC_SAW = 0, BASIC_SQR, BASIC_TRIANG, BASIC_SINE, LAST_SOUND };


/*----------------------------------------------------------------------------*/

typedef struct
{
	float_t 	amp;	// should be <= 1 for normal sound output
	float_t		last_amp;
	float_t 	freq;	// Hertz
	uint8_t 	syncin;//hardsync in
	uint8_t 	syncout;//hardsync out
	float_t 	phase;	// radians
	float_t		phi0;	// radians
	float_t		modInd;	// Modulation Index for FM
	float_t		mul;	// pitch frequency multiplier
	float_t 	out;	// output sample

} Oscillator_t;


/*Exported Functions-------------------------------------*/
float waveCompute(uint8_t select, float f0);
void osc_init(Oscillator_t * op, float_t amp, float_t freq);
float_t BasicSaw(Oscillator_t * op);
float_t BasicTriangle(Oscillator_t * op);




#endif /* INC_OSCILLATORS_H_ */

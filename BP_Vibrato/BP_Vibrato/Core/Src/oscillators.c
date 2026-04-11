/* oscillators.c */
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

#include "oscillators.h"


Oscillator_t 		op1;
Oscillator_t 		vibr_lfo;

/*===============================================================================================================*/

void
osc_init(Oscillator_t * op, float_t amp, float_t freq)
{
	op->amp = amp;
	op->last_amp = amp;
	op->syncin = 0;  //hardsync in
	op->syncout = 0;  //hardsync out
	op->freq = freq;
	op->phase = 0;
	op->out = 0;
	op->modInd = 0;
	op->mul = 1;
}
/*-------------------------------------------------------*/

void
OpSetFreq(Oscillator_t * op, float_t f)
{
	op->freq = f;
}

/*-------------------------------------------------------*/
float_t BasicSaw(Oscillator_t * op) // basic sawtooth
{
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->phase < _PI) op->out = 0.5f*op->amp * (op->phase / _PI);
	else op->out =  .5f*op->amp * (op->phase / _PI - 2);

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t BasicTriangle(Oscillator_t * op) // Naive Triangle
{
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->phase < _PI) op->out = op->amp * (-2/_PI * op->phase + 1);
	else op->out =  op->amp * (2/_PI * op->phase - 3);

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/


float waveCompute(float frq)
{
	float y;

	OpSetFreq(&op1, frq);

	y = 0.8f * BasicSaw(&op1);

	return y;
}

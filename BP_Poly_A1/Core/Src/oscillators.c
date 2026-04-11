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
Oscillator_t 		pwm_lfo;

Oscillator_t 		filt_lfo;


float pwm=0;

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
/*---------------------------------------------------------------*/
float_t BasicSquare(Oscillator_t * op) // naive square
{

	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->phase < _PI) op->out = op->amp ;
	else op->out = - op->amp ;

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t BasicSine(Oscillator_t * op) // accurate sine
{
	float_t z;

	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI) // keep phase in [0, 2pi]
		op->phase -= _2PI;

	z = sinf(op->phase);
	op->out = op->amp*z;

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t FLTR_Triangle_LFO(Oscillator_t * op) // Naive Triangle
{
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->phase < _PI) op->out = op->amp * (-1/_PI * op->phase + 1);
	else op->out =  op->amp * (1/_PI * op->phase - 1);

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t MorphingSaw(Oscillator_t * op) // Morphing sawtooth, tends to a triangle at high freqs
{
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->freq < 8000) op->phi0 = _PI/8000*(op->freq); else op->phi0 = _PI;
	if (op->phi0 <= .0001f) op->phi0 = .0001f;
	if (op->phase < op->phi0) op->out = op->amp * (-2/op->phi0 * op->phase + 1);
	else op->out =  op->amp * (2 * op->phase - _2PI - op->phi0)/(_2PI - op->phi0);

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t PulseWave(Oscillator_t * op) // PWM_naive square
{

	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;
	if (pwm> 0.16){
	if (op->phase < pwm) op->out = op->amp ;
	else op->out = - op->amp ;
	}
	else {
		if (op->phase < mapfloat(BasicSine(&pwm_lfo), -1, 1, .15, 6)) op->out = op->amp ;
		else op->out = - op->amp ;
	}


	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/


float waveCompute(uint8_t select, float frq)
{
	float y;

	OpSetFreq(&op1, frq);


			/* select an oscillator */
			switch (select)
			{
				case BASIC_SAW :
				y = BasicSaw(&op1); break;

				case BASIC_SQR :
				y = 0.4f * BasicSquare(&op1); break;
				//its a bit loud, so I multiply by 0.4f to lower its vol
				//so it matches the other osc's

				case BASIC_TRIANG :
				y = BasicTriangle(&op1); break;

				case BASIC_SINE :
				y = BasicSine(&op1); break;

				case MORPHSAW :
				y = MorphingSaw(&op1); break;

				case PULSE :
				y = 0.4 * PulseWave(&op1); break;



			default :
			y = 0;	break ;

		}

		return y;
	}

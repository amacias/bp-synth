/* waveGen.c */
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

#include "waveGen.h"

/*--------------------------------------------------------------*/
static float	f0;//base frequency

extern Oscillator_t 		op1 ;//our 1st oscillator!
extern uint8_t currNote;//from MIDI func in main.c

/*===============================================================================================================*/

//this will initialize basic settings to start the synth with
void Synth_Init(void)
{
	osc_init(&op1, 0.0f, 587.f);//inits the  &op1 oscillator
}

/*-------------THIS IS WHERE THE MAGIC HAPPENS!!!-------------------
----(this function will create the samples for the DMA buffer)----*/

void make_sound(uint16_t *buf , uint16_t length){
	uint16_t 	pos;
	uint16_t 	*outp;
	float	 	y = 0;
	float	 	yL, yR ;
	//float		change_in_note;
	uint16_t 	valueL, valueR;


	outp = buf;

	//f0=200.0;

	//MIDI note to frequency conversion equation
	f0=440.0 * powf(2.0, (float)(currNote - 69) * 0.08333333);
	//is same as--> 440 * ( 2^ ((midiNote - 69) * 0.08333333))

	for (pos = 0; pos < length; pos++)
	{

		/*--- Generate waveform ---*/
		y = waveCompute(f0);

		//make the left and right channel same values(mono output)
		yL = yR = y;

		/*--- clipping ---*/
		yL = (yL > 1.0f) ? 1.0f : yL; //clip too loud left samples
		yL = (yL < -1.0f) ? -1.0f : yL;

		yR = (yR > 1.0f) ? 1.0f : yR; //clip too loud right samples
		yR = (yR < -1.0f) ? -1.0f : yR;

		/****** let's hear the new sample *******/

		valueL = (uint16_t)((int16_t)((32767.0f) * yL)); // conversion float -> int
		valueR = (uint16_t)((int16_t)((32767.0f) * yR));

		*outp++ = valueL; // left channel sample
		*outp++ = valueR; // right channel sample

	}


}

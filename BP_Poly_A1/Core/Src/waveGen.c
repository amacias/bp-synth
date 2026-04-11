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
extern float vol;
static float env;

static float f0;//base frequency

extern Oscillator_t 	op1 ;//our 1st oscillator!
extern Oscillator_t 	vibr_lfo;//LFO osc for vibrato
extern Oscillator_t 	pwm_lfo;

extern Oscillator_t 		filt_lfo;

extern uint8_t currNote;//from MIDI func in main.c
extern float bendFactor;//the Pitchbend variable

extern ADSR_t 			amp_EG;
extern ADSR_t 			filterEG;

extern uint8_t    osc_sel;//oscillator selection


extern DSPfilter 	vcf;
extern float		filterFreq;

/*===============================================================================================================*/

//this will initialize basic settings to start the synth with
void Synth_Init(void)
{
	vol = 1;
	env = 0;
	ADSR_init(&amp_EG);
	ADSR_init(&filterEG);
	osc_init(&op1, 0.8f, 587.f);//inits the  &op1 oscillator
	osc_init(&vibr_lfo, 0, VIBRATO_FREQ);//inits vibr LFO and it's freq

	init_Filter();
	filterFreq = MIN_FREQ * powf(MAX_FREQ / MIN_FREQ, 1) / SAMPLERATE ;//open filter fully
	FilterRes_set(90);//gives default filter settings. may adjust in init later..

	pwm_lfo.amp=0.8;//sets pwm up
	pwm_lfo.freq=0.3;
	RelFltrEG_set(0);//start with NO Release on the filter
}

/*-------------THIS IS WHERE THE MAGIC HAPPENS!!!-------------------
----(this function will create the samples for the DMA buffer)----*/

void make_sound(uint16_t *buf , uint16_t length){
	uint16_t 	pos;
	uint16_t 	*outp;
	float	 	y = 0;
	float	 	yL, yR ;
	float 		f1;//adjusted frequency
	uint16_t 	valueL, valueR;


	outp = buf;

	//MIDI note to frequency conversion equation
	f0=440.0 * powf(2.0, (float)(currNote - 69) * 0.08333333) * bendFactor;

	/*--- vibrato modulation ---*/
	f1 = f0 *  (1+ vibr_lfo.out);

	/*--- Apply envelop ---*/
	env = ADSR_computeSample(&amp_EG);

	/*--- Update the cutoff value ---*/
	SetFilterValue((filterFreq * (1+ filt_lfo.out)) * (ADSR_computeSample(&filterEG)*22));
	//SetFilterValue((filterFreq * (1+ ADSR_computeSample(&filterEG)*22)));//no LFO


	for (pos = 0; pos < length; pos++)
	{

		/*--- vibrato lof calc ---*/
		BasicTriangle(&vibr_lfo);

		FLTR_Triangle_LFO(&filt_lfo);

		/*--- Generate waveform ---*/
		y = waveCompute(osc_sel, f1);



		y *= vol * env;//volume AND envelope together!

		/*--- Apply filter effect ---*/
		 if(vcf.type){ //<--completely stops any filter calculations if nil
		y = calcFilterSample(y);
		 }

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

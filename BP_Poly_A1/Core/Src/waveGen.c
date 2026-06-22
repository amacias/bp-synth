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

extern Oscillator_t 	op[]; 		// voice oscillators; op[v].amp==0 means inactive
extern float		voice_freq[];	// base MIDI frequencies; separate from op[].freq
					// which gets overwritten per-sample by waveComputeVoice
extern uint8_t		voice_fade[];	// 1 = voice doing a quick declick fade-out
extern Oscillator_t 	vibr_lfo;	// vibrato LFO
extern Oscillator_t 	pwm_lfo;

extern Oscillator_t 	filt_lfo;

extern float bendFactor;      // the Pitchbend variable
extern float osc1_coarse_mult;// CC11 coarse tune multiplier
extern float osc2_coarse_mult;// CC12 coarse tune multiplier

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

	// init all voice oscillators as inactive (amp=0)
	for (int v = 0; v < NUM_VOICES; v++) {
		osc_init(&op[v], 0.0f, 440.0f);
	}
	osc_init(&vibr_lfo, 0, VIBRATO_FREQ);//inits vibr LFO and its freq

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
	float 		f_base[NUM_VOICES];// per-voice base frequency (Hz * bendFactor)
	int			active;				// count of voices currently sounding
	uint16_t 	valueL, valueR;

	outp = buf;

	/*--- pre-compute per-voice base frequencies ---*/
	active = 0;
	for (int v = 0; v < NUM_VOICES; v++) {
		if (op[v].amp > 0.001f) {
			float coarse = (v < 2) ? osc1_coarse_mult : osc2_coarse_mult;
			f_base[v] = voice_freq[v] * bendFactor * coarse;
			active++;
		} else {
			f_base[v] = 0.0f;
		}
	}

	/*--- Apply envelope ---*/
	env = ADSR_computeSample(&amp_EG);
	/* EG release completed — silence all oscillators so their slots are free */
	if (amp_EG.state_ == DONE) {
		for (int v = 0; v < NUM_VOICES; v++) op[v].amp = 0.0f;
	}

	/*--- Update the cutoff value ---*/
	SetFilterValue((filterFreq * (1+ filt_lfo.out)) * (ADSR_computeSample(&filterEG)*22));

	for (pos = 0; pos < length; pos++)
	{
		/*--- LFO calculations ---*/
		BasicTriangle(&vibr_lfo);
		FLTR_Triangle_LFO(&filt_lfo);

		/*--- Sum all active voices with vibrato applied ---*/
		y = 0;
		for (int v = 0; v < NUM_VOICES; v++) {
			if (f_base[v] > 0.1f) {
				y += waveComputeVoice(osc_sel, f_base[v] * (1.0f + vibr_lfo.out), v);
			}
		}
		/* per-voice quick declick fade-out for single-note releases (legato mode) */
		for (int v = 0; v < NUM_VOICES; v++) {
			if (voice_fade[v]) {
				op[v].amp -= VOICE_FADE_STEP;
				if (op[v].amp <= 0.0f) {
					op[v].amp     = 0.0f;
					voice_fade[v] = 0;
				}
			}
		}
		/* normalize by active voice count to prevent output clipping */
		if (active > 0) y /= (float)active;

		y *= vol * env;//volume AND envelope together!

		/*--- Apply filter effect ---*/
		if(vcf.type){ //<--completely stops any filter calculations if nil
			y = calcFilterSample(y);
		}

		//make the left and right channel same values (mono output)
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

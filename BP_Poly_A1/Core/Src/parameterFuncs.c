/* *** parameterFuncs *** */
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

#include "parameterFuncs.h"
/* Private variables -----------------------------------------------*/
float vol;
uint8_t osc_sel;
float osc1_coarse_mult = 1.0f; // frequency multiplier from CC11 coarse tune
float osc2_coarse_mult = 1.0f; // frequency multiplier from CC12 coarse tune
extern Oscillator_t 		op[];
extern Oscillator_t 		vibr_lfo;
extern Oscillator_t 		pwm_lfo;

extern Oscillator_t 		filt_lfo;

extern float pwm;

/*-----------------------------------------------------------*/
void SynthOut_amp_set(uint8_t val){
  vol= (val/ MIDI_MAX)*(val/ MIDI_MAX);// creates a value of --> Zero and 0.007(val=1) up to 1(val=127)

}
/*-----------------------------------------------------------*/
void Vibrato_amp_set(uint8_t val){
  vibr_lfo.amp = (MAX_VIBRATO_AMP / MIDI_MAX * val)*(MAX_VIBRATO_AMP / MIDI_MAX * val);
}

/*-----------------------------------------------------------*/
void VibratoFreq_set(uint8_t val)
{
	vibr_lfo.freq = MAX_VIBRATO_FREQ / MIDI_MAX * val;
}
/*-------------------------------------------------------*/
void OSC_Select(uint8_t val)
{
	osc_sel = (uint8_t) rintf((LAST_SOUND - 1) / MIDI_MAX * val);
}
/*-----------------------------------------------------------*/
void square_dutycycle(uint8_t val){
	//val-=(-64);
	pwm= mapfloat(val, 0, 127, .15, 6);
}
/*---------------------------------------------------------------*/
void pwm_amp_set(uint8_t val){
	   pwm_lfo.amp= mapfloat(val, 0, 127, 0, 1);
}
/*---------------------------------------------------------------*/
void pwm_freq_set(uint8_t val){
	   pwm_lfo.freq= mapfloat(val, 0, 127, 0, 2.5);
}
/*---------------------------------------------------------------*/
void Filt1LFO_amp_set(uint8_t val)
{
	filt_lfo.amp =fscale(0, 127, 0, 32, val, -3.0f);
	//filt_lfo.amp = MAX_FILTER_LFO_AMP / MIDI_MAX * val;
}
/*-------------------------------------------------------*/
void Filt1LFO_freq_set(uint8_t val)
{
	filt_lfo.freq = MAX_VIBRATO_FREQ / MIDI_MAX * val;
}
/*-----------------------------------------------------------*/
void Osc1CoarseTune_set(uint8_t val)
{
    // CC11: voices 1-2; 0 = unison, 127 = +12 semitones (one octave up)
    osc1_coarse_mult = powf(2.0f, (float)val / 127.0f);
}
/*-----------------------------------------------------------*/
void Osc2CoarseTune_set(uint8_t val)
{
    // CC12: voices 3-4; 0 = unison, 127 = +12 semitones (one octave up)
    osc2_coarse_mult = powf(2.0f, (float)val / 127.0f);
}
/*-----------------------------------------------------------*/

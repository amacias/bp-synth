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
extern float vol;
extern Oscillator_t 		op1 ;
extern Oscillator_t 		vibr_lfo;

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
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

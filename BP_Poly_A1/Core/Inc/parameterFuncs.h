/* *** parameterFuncs.h *** */
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

#ifndef INC_PARAMETERFUNCS_H_
#define INC_PARAMETERFUNCS_H_

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "CONSTANTS.h"
#include "oscillators.h"
#include "MIDI_CCs.h"
#include "waveGen.h"

/*--------------------- Global Defines ------------------------ */

#define MAX_FILTER_LFO_AMP	1.0f
#define VIBRATO_AMP			0.01f
#define VIBRATO_FREQ		10.f
#define MAX_VIBRATO_AMP		0.5f
#define MAX_VIBRATO_FREQ	12.f

/*Exported Functions---------------------------------------*/

void SynthOut_amp_set(uint8_t val);
void Vibrato_amp_set(uint8_t val);
void VibratoFreq_set(uint8_t val);
void OSC_Select(uint8_t val);
void square_dutycycle(uint8_t val);
void pwm_amp_set(uint8_t val);
void pwm_freq_set(uint8_t val);

void Filt1LFO_freq_set(uint8_t val);
void Filt1LFO_amp_set(uint8_t val);










#endif /* INC_PARAMETERFUNCS_H_ */

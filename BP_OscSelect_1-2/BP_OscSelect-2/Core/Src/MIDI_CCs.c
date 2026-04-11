/*  ***MIDI_CCs.c*** */
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

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "MIDI_CCs.h"
/* Private function prototypes -----------------------------------------------*/
void ProcessRXdMidiCCs(void);


/* Private variables -----------------------------------------------*/
extern uint8_t MIDIcmd;
extern uint8_t MIDInote;
extern uint8_t MIDIvel;


/* Functions -----------------------------------------------*/
/*-----------------------------------------------------------------------------*/
void ProcessRXdMidiCCs()
{
  if(MIDIcmd==0xB0){

	uint8_t val = MIDIvel;


	switch(MIDInote) // CC number
		{

		case 1 :	Vibrato_amp_set(val);   break;//modwheel

    case 6 :  OSC_Select(val);         break;
		case 7 :	SynthOut_amp_set(val);   break;


		case 41 :	AttTime_set(val);			break;
		case 42 :	DecTime_set(val);			break;
		case 43 :	SustLevel_set(val);		break;
		case 44 :	RelTime_set(val);			break;


		case 60 :	VibratoFreq_set(val);	break;	// Vibrato frequency


    case 69 :   pwm_amp_set(val);		    break;
    case 70 :   pwm_freq_set(val);	    break;



    case 99 :   square_dutycycle(val);	break;

	  default:
	  break;

          }
        }
      }

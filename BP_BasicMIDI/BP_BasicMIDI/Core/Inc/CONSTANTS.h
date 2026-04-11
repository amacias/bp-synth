/* CONSTANTS.h */
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

#ifndef INC_CONSTANTS_H_
#define INC_CONSTANTS_H_
/*--------------------------------------------------------------------------------------*/

#define SAMPLERATE              32000


#define Ts						(1.f/SAMPLERATE)  // sample period
#define _2PI                    6.283185307f
#define _PI                    	3.14159265f

#define BUFF_LEN_DIV4           500 // number of samples for each channel(L&R) rendered to the buffer each time make_sound() is called.
#define BUFF_LEN_DIV2           (2*BUFF_LEN_DIV4)//used to offset mem location when rendering last half of buffer
#define BUFF_LEN                (4*BUFF_LEN_DIV4)  // Used to define the audiobuff[] length

#define MIDI_MAX				127.f 	// floating max value

#if defined   (__GNUC__)        /* GNU Compiler */
    #define __ALIGN    __attribute__ ((aligned (4)))
#endif /* __GNUC__ */

/************************************************************************************/


#endif /* INC_CONSTANTS_H_ */

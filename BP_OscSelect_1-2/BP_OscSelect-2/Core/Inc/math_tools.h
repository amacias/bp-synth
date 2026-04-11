/* math_tools.c */
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MATH_TOOLS_H
#define __MATH_TOOLS_H

/*-------------------------------------------------------*/

#include <math.h>
#include <stdint.h>
#include "CONSTANTS.h"

/*-------------------------------------------------------*/
float	Lin2Exp(uint8_t val, float min, float max); // input val in 0 ... 127
float 	tanhXdX(float x);
float 	fastTanh(float var);
float 	procClip(float in);
float 	fastTan(float x);
float   mapfloat(float val, float in_min, float in_max, float out_min, float out_max);

float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve);

/*-------------------------------------------------------*/
#endif /* __MATH_TOOLS_H */

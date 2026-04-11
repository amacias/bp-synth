/* math_tools.c*/
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

#include "math_tools.h"


//------------------------------------------------------------------------------------
// input val in 0...127
// output is an exponential between min and max
float	Lin2Exp(uint8_t val, float min, float max)
{
	return min * powf(max / min, val / MIDI_MAX);
}
//------------------------------------------------------------------------------------
float tanhXdX(float x)
{
	float a = x*x;
	// IIRC I got this as Pade-approx for tanh(sqrt(x))/sqrt(x)
	x = ((a + 105)*a + 945) / ((15*a + 420)*a + 945);
	return x;
}
//------------------------------------------------------------------------------------
float fastTanh(float var)
{
	if(var < -1.95f)     return -1.0f;
	else if(var > 1.95f) return  1.0f;
	else return  4.15f*var/(4.29f+var*var);
}
//------------------------------------------------------------------------------------
float 	procClip(float in)
{
	return in * tanhXdX(0.5*in);
}
//------------------------------------------------------------------------------------
float 	fastTan(float x)
{
	float A = -15*x+x*x*x;
	float B = 3*(-5+2*x*x);
	return A/B;
}
//--------------------------------------------------------------------------------------
float mapfloat(float val, float in_min, float in_max, float out_min, float out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//--------------------------------------------------------------------------------------
float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  uint8_t invFlag = 0;

  // condition curve parameter
  // limit range
  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;
  curve = (curve * -.1) ; // - invert and scale - positive numbers give more weight to high end on output
  curve = powf(10, curve); // convert linear scale into lograthimic exponent for other pow function

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Reference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (powf(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (powf(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}

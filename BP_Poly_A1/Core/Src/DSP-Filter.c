/* DSP-Filter.c */
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

//-----------------------------------------------------------------------------------
#include "DSP-Filter.h"

//-----------------------------------------------------------------------------------
DSPfilter vcf;
float filterFreq;

//------------------------------------------------------------------------------------
void init_Filter(void)
{
	vcf.type = 1;
	vcf.samp1 = 0;
	vcf.samp2 = 0;
	vcf.freq = 0.2f;
	vcf.q = 0.9f;
}

//------------------------------------------------------------------------------------
void 	FilterFreq_set(uint8_t val)
{
	filterFreq = MIN_FREQ * powf(MAX_FREQ / MIN_FREQ, val / MIDI_MAX) / SAMPLERATE ;
}

//------------------------------------------------------------------------------------
void 	FilterRes_set(uint8_t val)
{
	float fdback= val/MIDI_MAX;
	vcf.q = 1- fdback;
	if(vcf.q < 0.01f) vcf.q = 0.01f;
}

//------------------------------------------------------------------------------------
void 	FilterType_set(uint8_t val)
{
	vcf.type = (uint8_t)lrintf(2 * val / MIDI_MAX);
}
//------------------------------------------------------------------------------------
void SetFilterValue(float val)
{
	if (val > 0.73) val = 0.73f;//small adj could be made
	vcf.freq = val*0.5f;
	float gTn= _PI * vcf.freq;
	float X = -15*gTn+gTn*gTn*gTn;
	float Y = 3*(-5+2*gTn*gTn);
	vcf.gain = X/Y;
}

//------------------------------------------------------------------------------------
//the nitty gritty Filter function!
float calcFilterSample(float smpl)
{
	float out;
	const float fg 	= vcf.gain;
	float r;

  if (vcf.freq >= 0.4499f) r=1;
  else r= vcf.q;

	const float df = fg*fg;
	const float s = procClip( smpl * 0.8f);
	const float d0 = 1.f / (1.f + fg*2*r);
	const float samp1 = vcf.samp1;
	const float samp2 = vcf.samp2;
	const float fdb = df*d0;
	float y1=(fdb*s+samp2+fg*d0*samp1)/(fdb+1);
	const float dx = s - y1;
	const float y0 = (procClip(samp1) + fg*dx)*d0;

	vcf.samp1   = procClip(vcf.samp1) + 2*fg*(dx - 2*r*y0);
	vcf.samp2   = (vcf.samp2) + 2*fg*y0;

if(vcf.type==1)//low pass
	{
		if(y1 < -1.95f) out= -1.0f;
		else if(y1 > 1.95f) out= 1.0f;
		else out=  4.15f*y1/(4.29f+y1*y1);
	}

//we are only here if vcf.type was TRUE in waveGen.c
//so type is either a 1 or 2. Low or High.
else//hi pass
	{
		const float efg = 2*r*y0;
		const float h = s - efg - y1;
		out = h * FLTR_GAIN;
	}

	if (out > 1.0f) out=1.0f;
	if (out < -1.0f) out= -1.0f;
	return out;

}
//------------------------------------------------------------------------------------
float procClip(float n)
{
float a=0.5*n;
float b = a*a;
a = ((b + 105)*b + 945) / ((15*b + 420)*b + 945);
return n * a;
}
//------------------------------------------------------------------------------------

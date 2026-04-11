/* adsr.c */
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

#include "adsr.h"
/*---------------------------------------------------------------------------*/
ADSR_t			amp_EG;

float AttackVal=0;
float ReleaseVal=0;
/*---------------------------------------------------------------------------*/
void ADSR_init(ADSR_t *env)
{
  env->value_ = 0.0;
  env->attackRate_ = 0.1;
  env->decayRate_ = 0.1;
  env->sustainLevel_ = 0.5;
  env->releaseRate_ = 0.1;
  env->state_ = DONE;
}
/*---------------------------------------------------------------------------*/
void ADSR_keyOn(ADSR_t *env)
{
	env->state_ = ATTACK;
}

void ADSR_keyOff(ADSR_t *env)
{
	env->state_ = RELEASE;
}

/*---------------------------------------------------------------------------*/
void AttTime_set(uint8_t val)
{
	ADSR_setAttackTime(&amp_EG, val/MIDI_MAX + 0.0001f);
}
void DecTime_set(uint8_t val)
{
	ADSR_setDecayTime(&amp_EG, .35*val/MIDI_MAX + 0.0001f);
}
void SustLevel_set(uint8_t val)
{
	ADSR_setSustainLevel(&amp_EG, val/MIDI_MAX + 0.0001f);
}
void RelTime_set(uint8_t val)
{
	ADSR_setReleaseTime(&amp_EG, val/MIDI_MAX + 0.0001f);
}

/*---------------------------------------------------------------------------*/
void ADSR_setAttackTime(ADSR_t *env, float time)
{
	env->attackRate_ = 25 / ( time * SAMPLERATE );
}

void ADSR_setDecayTime(ADSR_t *env, float time)
{
	env->decayRate_ = 40 / ( time * SAMPLERATE );
}

void ADSR_setSustainLevel(ADSR_t *env, float level)
{
	if(env->state_==SUSTAIN){ //adjusts sustain level while playing
	  env->sustainLevel_ = level;
	  env->value_ = level;
	  return;
	  }
	env->sustainLevel_ = level;
}

void ADSR_setReleaseTime(ADSR_t *env, float time)
{
	env->releaseRate_ = 25 / ( time * SAMPLERATE );
}

/*---------------------------------------------------------------------------*/
float ADSR_computeSample(ADSR_t *env)
{

	switch (env->state_) {

  case ATTACK:
	  env->value_ += env->attackRate_;
    if (env->value_ >= 1.0f)
    {
    	env->value_ = 1.0f;
    	env->state_ = DECAY;
    }
    break;

  case DECAY:
	  env->value_ -= env->decayRate_;
    if (env->value_ <= env->sustainLevel_)
    {
    	env->value_ = env->sustainLevel_;
    	env->state_ = SUSTAIN;
    }
    break;

  case RELEASE:
	  env->value_ -= env->releaseRate_;
    if (env->value_ <= 0.0f)
    {
    	env->value_ =  0.0f;
    	env->state_ = DONE;
    }
  }

  return env->value_;


}
/*---------------------------------------------------------------------------*/





//end

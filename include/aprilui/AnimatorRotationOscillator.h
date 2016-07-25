/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_ROTATION_OSCILLATOR_H
#define APRILUI_ROTATION_OSCILLATOR_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport RotationOscillator : public Animator
		{
		public:
			RotationOscillator(chstr name);
			
			bool isAnimated();
			bool setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
			float getBaseline() { return mBaseline; }
			float getAmplitude() { return mAmplitude; }
			float getSpeed() { return mSpeed; }
			float getTimer() { return mTimer; }
			
		protected:
			float mBaseline;
			float mAmplitude;
			float mSpeed;
			float mTimer;
			float mInitialAngle;
			
		};
	}
}

#endif

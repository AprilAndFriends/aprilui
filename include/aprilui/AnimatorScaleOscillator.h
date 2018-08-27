/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_SCALE_OSCILLATOR_H
#define APRILUI_SCALE_OSCILLATOR_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport ScaleOscillator : public Animator
		{
		public:
			ScaleOscillator(chstr name);
			
			bool isAnimated();
			void notifyEvent(chstr eventName, void* params);
			bool setProperty(chstr name, chstr value);
			void update(float k);
			
		protected:
			float mBaseline;
			float mAmplitude;
			float mSpeed;
			float mTimer;
			gvec2f mInitialSize;
			
		};
	}
}

#endif

/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_MOVE_OSCILLATOR_H
#define APRILUI_MOVE_OSCILLATOR_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport MoveOscillator : public Animator
		{
		public:
			MoveOscillator(chstr name);
			
			bool isAnimated();
			void notifyEvent(chstr name, void* params);
			bool setProperty(chstr name, chstr value);
			void update(float k);
			
		protected:
			float mTimer;
			gvec2f mBaseline;
			gvec2f mAmplitude;
			gvec2f mSpeed;
			gvec2f mInitialPosition;
			
		};
	}
}

#endif

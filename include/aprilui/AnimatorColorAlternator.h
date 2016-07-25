/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_COLOR_ALTERNATOR_H
#define APRILUI_COLOR_ALTERNATOR_H

#include <april/RenderSystem.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		// note: only works on ColoredImages
		class apriluiExport ColorAlternator : public Animator
		{
		public:
			ColorAlternator(chstr name);
			
			bool isAnimated();
			void setSpeed(float value) { mSpeed = value; }
			bool setProperty(chstr name, chstr value);
			
			void update(float k);
			
		protected:
			april::Color mLow;
			april::Color mHigh;
			float mTimer;
			float mSpeed;
			
		};
	}
}

#endif

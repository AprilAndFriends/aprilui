/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_EARTHQUAKE_H
#define APRILUI_EARTHQUAKE_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport Earthquake : public Animator
		{
		public:
			Earthquake(chstr name);
			
			bool isAnimated();
			bool setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
		protected:
			gvec2f mInitial;
			float mIntensity;
			float mDuration;
			float mConstDuration;
			float mFreq;
			float mTimer;
			float mFreqTimer;
			
		};
	}
}

#endif

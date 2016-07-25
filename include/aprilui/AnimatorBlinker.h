/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_BLINKER_H
#define APRILUI_BLINKER_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport Blinker : public Animator
		{
		public:
			Blinker(chstr name);
			
			bool isAnimated();
			bool setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
		protected:
			float mDuration;
			float mFrequency;
			float mTimer;
			float mDelayTimer;
			float mDurationTimer;
			bool mStartVisibility;
			bool mEndVisibility;
			
		};
	}
}

#endif

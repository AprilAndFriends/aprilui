/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AnimatorBlinker.h"

namespace aprilui
{
	namespace Animators
	{
		Blinker::Blinker(chstr name) : Animator("Animators::Blinker", name, grect(0, 0, 1, 1))
		{
			mDuration = 0.0f;
			mTimer = 0.0f;
			mDelayTimer = 0.0f;
			mDurationTimer = 0.0f;
			mStartVisibility = false;
			mEndVisibility = false;
			mFrequency = 100.0f;
		}

		bool Blinker::isAnimated()
		{
			return true;
		}

		bool Blinker::setProperty(chstr name, chstr value)
		{
			if      (name == "delay")				mInitialDelay = mDelay = value;
			else if (name == "duration")			mDuration = value;
			else if (name == "freq")				mFrequency = value;
			else if (name == "start_visibility")	mStartVisibility = (bool)value;
			else if (name == "end_visibility")		mEndVisibility = (bool)value;
			return 1;
		}

		void Blinker::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mDelayTimer = mDelay;
				mDurationTimer = mDuration;
				mParent->setVisible(mStartVisibility);
			}
			Object::notifyEvent(name, params);
		}

		void Blinker::update(float k)
		{
			if (mDelayTimer > 0)
			{
				mDelayTimer -= k;
			}
			else if (mDuration >= 0)
			{
				mTimer -= k;
				if (mTimer < 0)
				{
					mParent->setVisible(!mParent->isVisible());
					mTimer = hrandf(1.0f) / mFrequency;
				}
				mDuration -= k;
				if (mDuration < 0)
				{
					mParent->setVisible(mEndVisibility);
				}
			}
		}
		
	}
}
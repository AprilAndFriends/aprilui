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

#include "AnimatorEArthquake.h"

namespace aprilui
{
	namespace Animators
	{
		Earthquake::Earthquake(chstr name) : Animator("Animators::Earthquake", name, grect(0, 0, 1, 1))
		{
			mIntensity = 0.0f;
			mDuration = 1.0f;
			mConstDuration = 1.0f;
			mFreq = 10.0f;
		}

		bool Earthquake::isAnimated()
		{
			return (mTimer <= mDuration);
		}

		void Earthquake::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mInitial = mParent->getPosition();
				mTimer = 0.0f;
				mFreqTimer = 0.0f;
			}
			Object::notifyEvent(name, params);
		}
		
		bool Earthquake::setProperty(chstr name,chstr value)
		{
			if      (name == "intensity")		mIntensity = value;
			else if (name == "duration")		mDuration = value;
			else if (name == "const_duration")	mConstDuration = value;
			else if (name == "freq")			mFreq = value;
			return 1;
		}
		
		void Earthquake::update(float k)
		{
			mTimer += k;
			mFreqTimer += k;
			float intensity = mIntensity;
			if (mTimer > mConstDuration)
			{
				intensity *= (mDuration - mTimer) / (mDuration - mConstDuration);
			}
			if (mTimer > mDuration)
			{
				mParent->setPosition(mInitial);
			}
			else if (mFreqTimer > 1.0f / mFreq)
			{
				mFreqTimer -= 1.0f / mFreq;
				mParent->setPosition(mInitial + gvec2((float)hrand((int)intensity), (float)hrand((int)intensity)));
			}
		}
		
	}
}
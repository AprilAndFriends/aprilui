/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorEArthquake.h"

namespace AprilUI
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
		
		void Earthquake::setProperty(chstr name,chstr value)
		{
			if      (name == "intensity")		mIntensity = value;
			else if (name == "duration")		mDuration = value;
			else if (name == "const_duration")	mConstDuration = value;
			else if (name == "freq")			mFreq = value;
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
				mParent->setPosition(mInitial + gvec2(hrand(intensity), hrand(intensity)));
			}
		}
		
	}
}

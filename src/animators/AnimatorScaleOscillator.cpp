/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AnimatorScaleOscillator.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		ScaleOscillator::ScaleOscillator(chstr name) : Animator("Animators::ScaleOscillator", name, grect(0, 0, 1, 1))
		{
			mBaseline = 0.0f;
			mAmplitude = 0.5f;
			mSpeed = 1.0f;
			mInitialSize.x = -10001;
			mInitialSize.y = -10001;
			mTimer = 0.0f;
		}

		bool ScaleOscillator::isAnimated()
		{
			return true;
		}

		bool ScaleOscillator::setProperty(chstr name, chstr value)
		{
			if      (name == "base")		mBaseline = value;
			else if (name == "amplitude")	mAmplitude = value;
			else if (name == "speed")		mSpeed = value;
			else if (name == "timer")		mTimer = value;
			else if (name == "delay")		mInitialDelay = mDelay = value;
			return 1;
		}

		void ScaleOscillator::notifyEvent(chstr eventName, void* params)
		{
			Object::notifyEvent(eventName, params);
		}

		void ScaleOscillator::update(float k)
		{
			if (mInitialSize.x < -10000)
			{
				mInitialSize = mParent->getSize();
			}
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			mTimer += k;
			float factor = dsin(mTimer * mSpeed * 360) * mAmplitude + mBaseline;
			mParent->setSize(mInitialSize.x + factor, mInitialSize.y + factor);
		}
		
	}
}

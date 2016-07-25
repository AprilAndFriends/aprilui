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

#include "AnimatorRotationOscillator.h"
#include "ObjectRotationImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		RotationOscillator::RotationOscillator(chstr name) : Animator("Animators::RotationOscillator", name, grect(0, 0, 1, 1))
		{
			mBaseline = 0.0f;
			mAmplitude = 0.5f;
			mSpeed = 360.0f;
			mInitialAngle = -10001.0f;
			mTimer = 0.0f;
		}

		bool RotationOscillator::isAnimated()
		{
			return true;
		}

		bool RotationOscillator::setProperty(chstr name, chstr value)
		{
			if      (name == "base")		mBaseline = value;
			else if (name == "amplitude")	mAmplitude = value;
			else if (name == "speed")		mSpeed = value;
			else if (name == "timer")		mTimer = value;
			else if (name == "delay")		mInitialDelay = mDelay = value;
			return 1;
		}

		void RotationOscillator::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				if (mInitialAngle < -10000)
				{
					mInitialAngle = ((RotationImageBox*)mParent)->getAngle();
				}
				else
				{
					update(0); // preserve angle
				}
			}
			Object::notifyEvent(name, params);
		}

		void RotationOscillator::update(float k)
		{
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			mTimer += k;
			float angle = (float)hsin(mTimer * mSpeed) * mAmplitude + mBaseline;
			((RotationImageBox*)mParent)->setAngle((float)fmod(angle * 360, 360));
		}
		
	}
}

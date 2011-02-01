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

		void RotationOscillator::setProperty(chstr name, chstr value)
		{
			if      (name == "base")		mBaseline = value;
			else if (name == "amplitude")	mAmplitude = value;
			else if (name == "speed")		mSpeed = value;
			else if (name == "timer")		mTimer = value;
			else if (name == "delay")		mInitialDelay = mDelay = value;
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
			float angle = dsin(mTimer * mSpeed) * mAmplitude + mBaseline;
			((RotationImageBox*)mParent)->setAngle(fmod(angle * 360, 360));
		}
		
	}
}

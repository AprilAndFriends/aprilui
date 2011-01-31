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

#include "AnimatorAlphaOscillator.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaOscillator::AlphaOscillator(chstr name) : Animator("Animators::Oscillator", name, grect(0, 0, 1, 1))
		{
			mBaseline = 0.5f;
			mAmplitude = 0.5f;
			mSpeed = 360.0f;
			mInitialAlpha = -10001.0f;
			mTimer = 0.0f;
		}

		bool AlphaOscillator::isAnimated()
		{
			return true;
		}

		void AlphaOscillator::setProperty(chstr name, chstr value)
		{
			if      (name == "base")		mBaseline = value;
			else if (name == "amplitude")	mAmplitude = value;
			else if (name == "speed")		mSpeed = value;
			else if (name == "delay")		mDelay = value;
		}

		void AlphaOscillator::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				if (mInitialAlpha < -10000.0f)
				{
					mInitialAlpha = mParent->getAlpha();
				}
				else
				{
					update(0); // preserve alpha
				}
			}
			Object::notifyEvent(name, params);
		}

		void AlphaOscillator::update(float k)
		{
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			mTimer += k;
			float alpha = dsin(mTimer * mSpeed) * mAmplitude + mBaseline;
			mParent->setAlpha(hclamp(alpha, 0.0f, 1.0f));
		}
		
	}
}

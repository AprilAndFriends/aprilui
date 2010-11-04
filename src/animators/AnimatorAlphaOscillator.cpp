/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorAlphaOscillator.h"

namespace AprilUI
{
	namespace Animators
	{
		AlphaOscillator::AlphaOscillator(chstr name) : Animator("Animators::Oscillator",name,0,0,1,1)
		{
			mBaseline=0.5; mAmplitude=0.5; mSpeed=360;
			mInitialAlpha=-10001;
			mTimer=0;
		}

		void AlphaOscillator::setProperty(chstr name,chstr value)
		{
			if      (name == "base") mBaseline=value;
			else if (name == "amplitude") mAmplitude=value;
			else if (name == "speed") mSpeed=value;
		}

		void AlphaOscillator::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				if (mInitialAlpha < -10000)
					mInitialAlpha=mParent->getAlpha();
				else
					update(0); // preserve alpha
			//	mTimer=0;
			}
			Object::notifyEvent(event_name,params);
		}

		void AlphaOscillator::update(float k)
		{
			mTimer+=k;
			float alpha=dsin(mTimer*mSpeed)*mAmplitude+mBaseline;
			mParent->setAlpha(hclamp(alpha,0.0f,1.0f));
		}
	}
}

/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorRotationOscillator.h"
#include "ObjectRotationImageBox.h"

namespace AprilUI
{
	namespace Animators
	{
		RotationOscillator::RotationOscillator(chstr name) : Animator("Animators::RotationOscillator",name,0,0,1,1)
		{
			mBaseline=0.0; mAmplitude=0.5; mSpeed=360;
			mInitialAngle=-10001;
			mTimer=0;
		}

		void RotationOscillator::setProperty(chstr name,chstr value)
		{
			if      (name == "base") mBaseline=value;
			else if (name == "amplitude") mAmplitude=value;
			else if (name == "speed") mSpeed=value;
			else if (name == "timer") mTimer=value;
		}

		void RotationOscillator::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				if (mInitialAngle < -10000)
					mInitialAngle=((RotationImageBox*)mParent)->getAngle();
				else
					update(0); // preserve angle
			//	mTimer=0;
			}
			Object::notifyEvent(event_name,params);
		}

		void RotationOscillator::update(float k)
		{
			mTimer+=k;
			float angle=dsin(mTimer*mSpeed)*mAmplitude+mBaseline;
			((RotationImageBox*)mParent)->setAngle(fmod(angle*360,360));
		}
	}
}

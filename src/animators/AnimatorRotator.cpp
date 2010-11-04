/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <math.h>

#include <hltypes/hstring.h>

#include "AnimatorRotator.h"
#include "ObjectRotationImageBox.h"

namespace AprilUI
{
	namespace Animators
	{
		Rotator::Rotator(chstr name) : Animator("Animators::Scaler",name,0,0,1,1)
		{
			mAccel=mSpeed=0;
			mInitialSpeed=-10000;
			mInitialAngle=-10000001;
		}

		void Rotator::setProperty(chstr name,chstr value)
		{
			if      (name == "speed") mSpeed=mInitialSpeed=value;
			else if (name == "accel") mAccel=value;
		}

		void Rotator::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				if (mInitialAngle < -1000000)
					mInitialAngle=((RotationImageBox*) mParent)->getAngle();
				else
					((RotationImageBox*) mParent)->setAngle(mInitialAngle);
				mSpeed=mInitialSpeed;
			}
			Object::notifyEvent(event_name,params);
		}

		void Rotator::update(float k)
		{
			float angle=((RotationImageBox*) mParent)->getAngle();
			if (fabs(mAccel) > 0.01f)
			{
				mSpeed+=mAccel*k;
			}
			angle+=k*mSpeed;
			
			((RotationImageBox*) mParent)->setAngle(angle);
		}
	}
}

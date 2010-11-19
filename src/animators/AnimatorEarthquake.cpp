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

#include "AnimatorEArthquake.h"

namespace AprilUI
{
	namespace Animators
	{
		Earthquake::Earthquake(chstr name) : Animator("Animators::Earthquake",name,0,0,1,1)
		{
			mIntensity=0;
			mDuration=1;
			mConstDuration=1;
			mFreq=10;
		}

		void Earthquake::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mInitialX=mParent->getXPosition();
				mInitialY=mParent->getYPosition();
				mTimer=mFreqTimer=0;
			}
			Object::notifyEvent(event_name,params);
		}
		
		void Earthquake::setProperty(chstr name,chstr value)
		{
			if      (name == "intensity") mIntensity=value;
			else if (name == "duration")  mDuration=value;
			else if (name == "const_duration")  mConstDuration=value;
			else if (name == "freq")  mFreq=value;
		}
		
		void Earthquake::update(float k)
		{
			mTimer+=k; mFreqTimer+=k;
			float intensity=mIntensity;
			if (mTimer > mConstDuration)
			{
				intensity*=(mDuration-mTimer)/(mDuration-mConstDuration);
			}
			if (mTimer > mDuration)
			{
				mParent->setPosition(mInitialX,mInitialY);
			}
			else if (mFreqTimer > 1.0f/mFreq)
			{
				mFreqTimer-=1/mFreq;
				mParent->setPosition(mInitialX+hrand(intensity),mInitialY+hrand(intensity));
			}
		}
	}
}

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

#include "Animators.h"

namespace AprilUI
{
	namespace Animators
	{
		Blinker::Blinker(chstr name) : Object("Animators::Blinker",name,0,0,1,1)
		{
			mDelay=mDuration=mTimer=mDelayTimer=mDurationTimer=0;
			mStartVisibility=mEndVisibility=0;
			mFrequency=100;
		}

		void Blinker::setProperty(chstr name,chstr value)
		{
			if      (name == "delay")    mDelay=value;
			else if (name == "duration") mDuration=value;
			else if (name == "freq")     mFrequency=value;
			else if (name == "start_visibility") mStartVisibility=((int) value)!=0;
			else if (name == "end_visibility")   mEndVisibility=((int) value)!=0;

		}

		void Blinker::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mDelayTimer=mDelay;
				mDurationTimer=mDuration;
				mParent->setVisible(mStartVisibility);
			}
			Object::notifyEvent(event_name,params);
		}

		void Blinker::update(float k)
		{
			if (mDelayTimer > 0) mDelayTimer-=k;
			else if (mDuration >= 0)
			{
				mTimer-=k;
				if (mTimer < 0)
				{
					mParent->setVisible(!mParent->isVisible());
					mTimer=((rand()%10000)/10000.0f)/mFrequency;
				}
				mDuration-=k;
				if (mDuration < 0)
				{
					mParent->setVisible(mEndVisibility);
				}
			}
		}
	}
}

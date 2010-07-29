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

#include "Animators.h"

namespace AprilUI
{
	namespace Animators
	{
		AlphaFader::AlphaFader(chstr name) : Object("Animators::Scaler",name,0,0,1,1)
		{
			mAccel=mSpeed=mInitialSpeed=mDelay=mTimer=0;
			mDestAlpha=-10000;
			mInitialAlpha=-10001;
		}

		void AlphaFader::setProperty(chstr name,chstr value)
		{
			if      (name == "speed") mSpeed=mInitialSpeed=value;
			else if (name == "accel") mAccel=value;
			else if (name == "delay") mDelay=value;
			else if (name == "dest_alpha") mDestAlpha=value;
		}

		void AlphaFader::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				if (mInitialAlpha < -10000)
					mInitialAlpha=mParent->getAlpha();
				else
					mParent->setAlpha(mInitialAlpha);
					
				if (mDelay) mTimer=mDelay;
				mSpeed=mInitialSpeed;
			}
			Object::notifyEvent(event_name,params);
		}

		void AlphaFader::update(float k)
		{
			if (mTimer > 0) { mTimer-=k; return; }
			float alpha=mParent->getAlpha();
			if (alpha == mDestAlpha) return;
			float prevalpha=alpha;
			if (fabs(mAccel) > 0.01f)
				mSpeed+=mAccel*k;
			
			alpha+=k*mSpeed;
			if (sgn(mDestAlpha-alpha) != sgn(mDestAlpha-prevalpha)) alpha=mDestAlpha;

			mParent->setAlpha(std::max(0.0f,std::min(1.0f,alpha)));
		}
	}
}

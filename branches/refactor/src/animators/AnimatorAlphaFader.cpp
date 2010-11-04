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

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorAlphaFader.h"

namespace AprilUI
{
	namespace Animators
	{
		AlphaFader::AlphaFader(chstr name) : Animator("Animators::Scaler",name,grect(0,0,1,1))
		{
			mAccel=mSpeed=mInitialSpeed=mDelay=mTimer=0;
			mDestAlpha=-10000;
			mInitialAlpha=-10001;
		}
		
		void AlphaFader::reset()
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
		
		void AlphaFader::fade(float dest,float time)
		{
			mDestAlpha=dest;
			float diff=mDestAlpha-mParent->getAlpha();
			mSpeed=sgn(diff)*fabs(diff)/time;
			mAccel=0; mDelay=0;
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
			if (sgn(mDestAlpha-alpha) != sgn(mDestAlpha-prevalpha))
			{
				alpha=mDestAlpha;
				reset();
			}

			mParent->setAlpha(hclamp(alpha,0.0f,1.0f));
		}
	}
}

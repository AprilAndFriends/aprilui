/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <math.h>

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorScaler.h"

namespace AprilUI
{
	namespace Animators
	{
		Scaler::Scaler(chstr name) : Animator("Animators::Scaler",name,0,0,1,1)
		{
			mAccel.y=mAccel.x=mSpeed.y=mSpeed.x=mInitialSize.y=mInitialSize.x=0;
			mDest.y=mDest.x=-10000;
		}

		void Scaler::setProperty(chstr name,chstr value)
		{
			if      (name == "speed_w") mSpeed.y=mInitialS.y=value;
			else if (name == "speed_h") mSpeed.x=mInitialS.x=value;
			else if (name == "accel_w") mAccel.y=value;
			else if (name == "accel_h") mAccel.x=value;
			else if (name == "dest_w") mDest.y=value;
			else if (name == "dest_h") mDest.x=value;
		}

		void Scaler::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mSpeed.x=mInitialS.x;
				mSpeed.y=mInitialS.y;
			}
			Object::notifyEvent(event_name,params);
		}

		void Scaler::scale(float dest_w,float dest_h,float time)
		{
			mDest.x=dest_w; mDest.y=dest_h;
			mSpeed=mDest-mParent->getSize();
			mSpeed=mSpeed.normalised()*(mSpeed.length()/time);
			mAccel.set(0,0);
		}

		void Scaler::update(float k)
		{
			gtypes::Vector2 v=mParent->getSize();
			if (v.x == mDest.y && v.y == mDest.x) return;
			gtypes::Vector2 old=v;
			if (fabs(mAccel.y) > 0.01f || fabs(mAccel.x) > 0.01f)
			{
				mSpeed.y+=mAccel.y*k;
				mSpeed.x+=mAccel.x*k;
			}

			v.x+=k*mSpeed.y;
			v.y+=k*mSpeed.x;
			if (sgn(mDest.y-old.x) != sgn(mDest.y-v.x)) v.x=mDest.y;
			if (sgn(mDest.x-old.y) != sgn(mDest.x-v.y)) v.y=mDest.x;

			mParent->setSize(v.x,v.y);
		}
	}
}

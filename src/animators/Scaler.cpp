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

#include "Animators.h"

namespace AprilUI
{
	namespace Animators
	{
		Scaler::Scaler(chstr name) : Object("Animators::Scaler",name,0,0,1,1)
		{
			mAccelW=mAccelH=mSpeedW=mSpeedH=mInitialW=mInitialH=0;
			mDestW=mDestH=-10000;
		}

		void Scaler::setProperty(chstr name,chstr value)
		{
			if      (name == "speed_w") mSpeedW=mInitialSW=value;
			else if (name == "speed_h") mSpeedH=mInitialSH=value;
			else if (name == "accel_w") mAccelW=value;
			else if (name == "accel_h") mAccelH=value;
			else if (name == "dest_w") mDestW=value;
			else if (name == "dest_h") mDestH=value;
		}

		void Scaler::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mSpeedH=mInitialSH;
				mSpeedW=mInitialSW;
			}
			Object::notifyEvent(event_name,params);
		}

		void Scaler::update(float k)
		{
			gtypes::Vector2 v=mParent->getSize();
			if (v.x == mDestW && v.y == mDestH) return;
			gtypes::Vector2 old=v;
			if (fabs(mAccelW) > 0.01f || fabs(mAccelH) > 0.01f)
			{
				mSpeedW+=mAccelW*k;
				mSpeedH+=mAccelH*k;
			}

			v.x+=k*mSpeedW;
			v.y+=k*mSpeedH;
			if (sgn(mDestW-old.x) != sgn(mDestW-v.x)) v.x=mDestW;
			if (sgn(mDestH-old.y) != sgn(mDestH-v.y)) v.y=mDestH;

			mParent->setSize(v.x,v.y);
		}
	}
}

/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "Animators.h"

namespace AprilUI
{
	namespace Animators
	{
		Mover::Mover(chstr name) : Object("Animators::Mover",name,0,0,1,1)
		{
			mAccelX=mAccelY=mSpeedX=mSpeedY=mInitialSY=mInitialSX=0;
			mDestX=mDestY=-10000;
		}

		void Mover::setProperty(chstr name,chstr value)
		{
			if      (name == "speed_x") mSpeedX=mInitialSX=value;
			else if (name == "speed_y") mSpeedY=mInitialSY=value;
			else if (name == "accel_x") mAccelX=value;
			else if (name == "accel_y") mAccelY=value;
			else if (name == "dest_x")  mDestX=value;
			else if (name == "dest_y")  mDestY=value;
		}

		void Mover::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mSpeedX=mInitialSX;
				mSpeedY=mInitialSY;
			}
			Object::notifyEvent(event_name,params);
		}

		void Mover::update(float k)
		{
			gtypes::Vector2 v=mParent->getPosition();
			if (v.x == mDestX && v.y == mDestY) return;
			gtypes::Vector2 old=v;
			if (fabs(mAccelX) > 0.01f || fabs(mAccelY) > 0.01f)
			{
				mSpeedX+=mAccelX*k;
				mSpeedY+=mAccelY*k;
			}

			v.x+=k*mSpeedX;
			v.y+=k*mSpeedY;
			if (sgn(mDestX-old.x) != sgn(mDestX-v.x)) v.x=mDestX;
			if (sgn(mDestY-old.y) != sgn(mDestY-v.y)) v.y=mDestY;
			
			mParent->setPosition(v);
		}
	}
}

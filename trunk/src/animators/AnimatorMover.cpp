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

#include "AnimatorMover.h"

namespace AprilUI
{
	namespace Animators
	{
		Mover::Mover(chstr name) : Object("Animators::Mover",name,0,0,1,1)
		{
			mAccel.x=mAccel.y=mSpeed.x=mSpeed.y=mInitialS.y=mInitialS.x=0;
			mDest.x=mDest.y=-10000;
		}

		void Mover::setProperty(chstr name,chstr value)
		{
			if      (name == "speed_x") mSpeed.x=mInitialS.x=value;
			else if (name == "speed_y") mSpeed.y=mInitialS.y=value;
			else if (name == "accel_x") mAccel.x=value;
			else if (name == "accel_y") mAccel.y=value;
			else if (name == "dest_x")  mDest.x=value;
			else if (name == "dest_y")  mDest.y=value;
		}

		void Mover::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mSpeed.x=mInitialS.x;
				mSpeed.y=mInitialS.y;
			}
			Object::notifyEvent(event_name,params);
		}
		
		void Mover::move(float dest_x,float dest_y,float speed)
		{
			mDest.x=dest_x; mDest.y=dest_y;
			mSpeed=mDest-mParent->getPosition();
			mSpeed=mSpeed.normalised()*(mSpeed.length()/speed);
			mAccel.set(0,0);
		}

		void Mover::update(float k)
		{
			gtypes::Vector2 v=mParent->getPosition();
			if (v.x == mDest.x && v.y == mDest.y) return;
			gtypes::Vector2 old=v;
			v.x+=k*mSpeed.x;
			v.y+=k*mSpeed.y;
			if (sgn(mDest.x-old.x) != sgn(mDest.x-v.x)) v.x=mDest.x;
			if (sgn(mDest.y-old.y) != sgn(mDest.y-v.y)) v.y=mDest.y;
			if (fabs(mAccel.x) > 0.01f || fabs(mAccel.y) > 0.01f)
			{
				mSpeed.x+=mAccel.x*k;
				mSpeed.y+=mAccel.y*k;
			}
			
			mParent->setPosition(v);
		}
	}
}

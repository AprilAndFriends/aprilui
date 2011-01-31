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
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorMover.h"

namespace aprilui
{
	namespace Animators
	{
		Mover::Mover(chstr name) : Animator("Animators::Mover", name, grect(0, 0, 1, 1))
		{
			mDest.x = -10000.0f;
			mDest.y = -10000.0f;
		}

		bool Mover::isAnimated()
		{
			return (mParent->getPosition() != mDest);
		}

		void Mover::setProperty(chstr name, chstr value)
		{
			if      (name == "speed_x")	mSpeed.x = mInitialS.x = value;
			else if (name == "speed_y")	mSpeed.y = mInitialS.y = value;
			else if (name == "accel_x")	mAccel.x = value;
			else if (name == "accel_y")	mAccel.y = value;
			else if (name == "dest_x")	mDest.x = value;
			else if (name == "dest_y")	mDest.y = value;
            else if (name == "delay")	mDelay = value;
		}

		void Mover::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mSpeed = mInitialS;
			}
			Object::notifyEvent(name, params);
		}
		
		void Mover::move(float dest_x, float dest_y, float time)
		{
			mDest = gvec2(dest_x, dest_y);
			mSpeed = mDest - mParent->getPosition();
			mSpeed = mSpeed.normalised() * (mSpeed.length() / time);
			mAccel.set(0, 0);
		}

		void Mover::update(float k)
		{
			gvec2 v = mParent->getPosition();
			if (v.x == mDest.x && v.y == mDest.y)
			{
				return;
			}
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			gvec2 old = v;
			v += mSpeed * k;
			if (sgn(mDest.x - old.x) != sgn(mDest.x - v.x) || sgn(mDest.y - old.y) != sgn(mDest.y - v.y))
			{
				v.x = mDest.x;
				v.y = mDest.y;
			}
			if (fabs(mAccel.x) > 0.01f || fabs(mAccel.y) > 0.01f)
			{
				mSpeed += mAccel * k;
			}
			mParent->setPosition(v);
		}
		
	}
}

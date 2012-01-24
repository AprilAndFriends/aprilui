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
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AnimatorScaler.h"

namespace aprilui
{
	namespace Animators
	{
		Scaler::Scaler(chstr name) : Animator("Animators::Scaler", name, grect(0, 0, 1, 1))
		{
			mAccel.y = 0.0f;
			mAccel.x = 0.0f;
			mSpeed.y = 0.0f;
			mSpeed.x = 0.0f;
			mInitialSize.y = 0.0f;
			mInitialSize.x = 0.0f;
			mDest.y = -10000.0f;
			mDest.x = -10000.0f;
			mStart.set(-10000.0f, -10000.0f);
			mT.set(0,0);
		}

		bool Scaler::isAnimated()
		{
			return (mParent->getSize() != mDest);
		}

		void Scaler::setProperty(chstr name, chstr value)
		{
			if      (name == "speed_w")	mSpeed.x = mInitialS.x = value;
			else if (name == "speed_h")	mSpeed.y = mInitialS.y = value;
			else if (name == "accel_w")	mAccel.x = value;
			else if (name == "accel_h")	mAccel.y = value;
			else if (name == "dest_w")	mDest.x = value;
			else if (name == "dest_h")	mDest.y = value;
            else if (name == "delay")	mInitialDelay = mDelay = value;
		}

		void Scaler::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mSpeed = mInitialS;
			}
			Object::notifyEvent(name, params);
		}

		void Scaler::scale(float dest_w, float dest_h, float time)
		{
			mDest = gvec2(dest_w, dest_h);
			mStart = mParent->getSize();
			mSpeed = mDest - mStart;
			mSpeed = mSpeed.normalized() * (mSpeed.length() / time);
			mAccel.set(0, 0);
			mT.set(0,0);
		}

		void Scaler::update(float k)
		{
			if (mSpeed.x == 0 && mSpeed.y == 0) return;
			gvec2 v = mParent->getSize();
			if (v.x == mDest.x && v.y == mDest.y)
			{
				return;
			}
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			if (fabs(mAccel.y) > 0.01f || fabs(mAccel.x) > 0.01f)
			{
				mSpeed += mAccel * k;
			}
			if (mStart.x == -10000.0f)
				mStart = mParent->getSize();
			if (mDest.x == -10000.0f)
				mDest = mStart + mSpeed * 100000;		
			gvec2 vec = mDest - mStart;
			float speedx = vec.x == 0 ? 0 : fabs(mSpeed.x / vec.x);
			float speedy = vec.y == 0 ? 0 : fabs(mSpeed.y / vec.y);
			mT.x += k * speedx;
			mT.y += k * speedy;
			if (mT.x < 1.0f && mT.y < 1.0f)
				v = mStart + gvec2(vec.x*mT.x, vec.y*mT.y);
			else
				v = mDest;
			mParent->setSize(v);
		}
	}
}

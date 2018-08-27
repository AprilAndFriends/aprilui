/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AnimatorMover.h"

namespace aprilui
{
	namespace Animators
	{
		Mover::Mover(chstr name) : Animator("Animators::Mover", name, grectf(0, 0, 1, 1))
		{
			mDest.x = -10000.0f;
			mDest.y = -10000.0f;
			mStart.set(-10000.0f,-10000.0f);
		}

		bool Mover::isAnimated()
		{
			return (mParent->getPosition() != mDest);
		}

		bool Mover::setProperty(chstr name, chstr value)
		{
			if      (name == "speed_x")	mSpeed.x = mInitialS.x = value;
			else if (name == "speed_y")	mSpeed.y = mInitialS.y = value;
			else if (name == "accel_x")	mAccel.x = value;
			else if (name == "accel_y")	mAccel.y = value;
			else if (name == "dest_x")	mDest.x = value;
			else if (name == "dest_y")	mDest.y = value;
            else if (name == "delay")	mInitialDelay = mDelay = value;
			return 1;
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
			mDest = gvec2f(dest_x, dest_y);
			mStart = mParent->getPosition();
			mSpeed = mDest - mStart;
			mSpeed = mSpeed.normalized() * (mSpeed.length() / time);
			mAccel.set(0, 0);
			mT.set(0, 0);
		}

		void Mover::update(float k)
		{
			if (mSpeed.x == 0 && mSpeed.y == 0) return;
			gvec2f v = mParent->getPosition();
			if (v.x == mDest.x && v.y == mDest.y)
			{
				return;
			}
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }

			if (fabs(mAccel.x) > 0.01f || fabs(mAccel.y) > 0.01f)
			{
				mSpeed += mAccel * k;
			}

			if (mStart.x == -10000.0f)
				mStart = mParent->getPosition();
			if (mDest.x == -10000.0f)
				mDest = mStart + mSpeed * 100000;
			gvec2f vec = mDest - mStart;
			float speedx = vec.x == 0 ? 0 : fabs(mSpeed.x / vec.x);
			float speedy = vec.y == 0 ? 0 : fabs(mSpeed.y / vec.y);
			mT.x += k * speedx;
			mT.y += k * speedy;
			if (mT.x < 1.0f && mT.y < 1.0f)
				v = mStart + gvec2f(vec.x*mT.x, vec.y*mT.y);
			else
				v = mDest;
			mParent->setPosition(v);
		}
		
	}
}

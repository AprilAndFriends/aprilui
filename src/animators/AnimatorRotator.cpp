/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AnimatorRotator.h"
#include "ObjectRotationImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		Rotator::Rotator(chstr name) : Animator("Animators::Rotator", name, grectf(0, 0, 1, 1))
		{
			mAccel = 0.0f;
			mSpeed = 0.0f;
			mInitialSpeed = -10000.0f;
			mInitialAngle = -10000001.0f;
			mDestAngle = -1.0f;
		}

		bool Rotator::isAnimated()
		{
			return true;
		}

		bool Rotator::setProperty(chstr name, chstr value)
		{
			if      (name == "speed")		mSpeed = mInitialSpeed = value;
			else if (name == "accel")		mAccel = value;
			else if (name == "delay")		mInitialDelay = mDelay = value;
			else if (name == "dest_angle")	mDestAngle = fmod((float)value, 360.0f);
			return 1;
		}

		void Rotator::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				if (mInitialAngle < -1000000.0f)
				{
					mInitialAngle = ((RotationImageBox*)mParent)->getAngle();
				}
				else
				{
					((RotationImageBox*)mParent)->setAngle(mInitialAngle);
				}
				mSpeed = mInitialSpeed;
			}
			Object::notifyEvent(name, params);
		}

		void Rotator::update(float k)
		{
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			RotationImageBox* imageBox = (RotationImageBox*)mParent;
			float angle = imageBox->getAngle();
			if (fabs(angle - mDestAngle) < 0.01f)
			{
				imageBox->setAngle(mDestAngle);
				return;
			}
			if (fabs(mAccel) > 0.01f)
			{
				mSpeed += mAccel * k;
			}
			float oldAngle = angle;
			angle += k * mSpeed;
			int circles = (int)floor(angle / 360);
			if ((mDestAngle >= 0.0f && hsgn(oldAngle + 360 * circles - mDestAngle) != hsgn(angle + 360 * circles - mDestAngle)) ||
				(hsgn(oldAngle - 360 * circles - mDestAngle) != hsgn(angle - 360 * circles - mDestAngle)))
			{
				angle = mDestAngle;
			}
			imageBox->setAngle(fmod(angle, 360.0f));
		}
		
	}
}

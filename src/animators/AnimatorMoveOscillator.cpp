/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AnimatorMoveOscillator.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		MoveOscillator::MoveOscillator(chstr name) : Animator("Animators::MoveOscillator", name, grect(0, 0, 1, 1))
		{
			mBaseline.x = 0.0f;
			mBaseline.y = 0.0f;
			mAmplitude.x = 0.5f;
			mAmplitude.y = 0.5f;
			mSpeed.x = 1.0f;
			mSpeed.y = 1.0f;
			mInitialPosition.x = -10001;
			mInitialPosition.y = -10001;
			mTimer = 0.0f;
		}

		bool MoveOscillator::isAnimated()
		{
			return true;
		}

		void MoveOscillator::setProperty(chstr name, chstr value)
		{
			if      (name == "base_x")		mBaseline.x = value;
			else if (name == "base_y")		mBaseline.y = value;
			else if (name == "amplitude_x")	mAmplitude.x = value;
			else if (name == "amplitude_y")	mAmplitude.y = value;
			else if (name == "speed_x") 	mSpeed.x = value;
			else if (name == "speed_y") 	mSpeed.y = value;
			else if (name == "timer")		mTimer = value;
			else if (name == "delay")		mInitialDelay = mDelay = value;
		}

		void MoveOscillator::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				if (mInitialPosition.x < -10000.0f)
				{
					mInitialPosition.x = mParent->getX();
				}
				if (mInitialPosition.y < -10000.0f)
				{
					mInitialPosition.y = mParent->getY();
				}
				update(0);
			}
			Object::notifyEvent(name, params);
		}

		void MoveOscillator::update(float k)
		{
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			mTimer += k;
			float factor_x = dsin(mTimer * mSpeed.x * 360) * mAmplitude.x + mBaseline.x;
			float factor_y = dsin(mTimer * mSpeed.y * 360) * mAmplitude.y + mBaseline.y;
			mParent->setPosition(mInitialPosition.x + factor_x, mInitialPosition.y + factor_y);
		}
		
	}
}

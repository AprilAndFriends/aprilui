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
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AnimatorAlphaFader.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaFader::AlphaFader(chstr name) : Animator("Animators::Scaler", name, grect(0, 0, 1, 1))
		{
			mInitialAccel = mAccel = 0.0f;
			mInitialSpeed = mSpeed = 0.0f;
			mInitialDestAlpha = mDestAlpha = -10000.0f;
			mInitialAlpha = -10001.0f;
			mResetHack = false;
		}
		
		void AlphaFader::reset()
		{
			if (mResetHack)
			{
				mAccel = mInitialAccel;
				mSpeed = mInitialSpeed;
				mDelay = mInitialDelay;
				mDestAlpha = mInitialDestAlpha;
			}
		}
		
		bool AlphaFader::isAnimated()
		{
			return (mParent->getAlpha() != mDestAlpha);
		}

		void AlphaFader::setProperty(chstr name, chstr value)
		{
			if      (name == "speed")		mInitialSpeed = mSpeed = value;
			else if (name == "accel")		mInitialAccel = mAccel = value;
			else if (name == "delay")		mInitialDelay = mDelay = value;
			else if (name == "dest_alpha")	mInitialDestAlpha = mDestAlpha = value;
			else if (name == "reset_hack")	mResetHack = (bool)value;
		}

		void AlphaFader::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				if (mInitialAlpha < -10000.0f)
				{
					mInitialAlpha = mParent->getAlpha();
				}
				else
				{
					mParent->setAlpha(mInitialAlpha);
				}
			}
			Object::notifyEvent(name,params);
		}
		
		void AlphaFader::fade(float dest, float time)
		{
			mDestAlpha = dest;
			float diff = mDestAlpha - mParent->getAlpha();
			mSpeed = hsgn(diff) * fabs(diff) / time;
			mAccel = 0.0f;
			mDelay = 0.0f;
		}
		
		void AlphaFader::update(float k)
		{
			if (mDelay > 0.0f)
			{
				mDelay -= k;
				return;
			}
			float alpha = mParent->getAlpha();
			if (alpha == mDestAlpha)
			{
				return;
			}
			float prevalpha = alpha;
			if (fabs(mAccel) > 0.01f)
			{
				mSpeed += mAccel * k;
			}
			alpha += k * mSpeed;
			if (hsgn(mDestAlpha - alpha) != hsgn(mDestAlpha - prevalpha))
			{
				alpha = mDestAlpha;
			}
			mParent->setAlpha(hclamp(alpha, 0.0f, 1.0f));
		}
		
	}
}

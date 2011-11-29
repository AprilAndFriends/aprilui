/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "aprilui.h"
#include "AnimatorBlueChanger.h"

namespace aprilui
{
	namespace Animators
	{
		BlueChanger::BlueChanger(chstr name) : Animator(name)
		{
		}

		Animator* BlueChanger::createInstance(chstr name)
		{
			return new BlueChanger(name);
		}

		void BlueChanger::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = mParent->getBlue();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void BlueChanger::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			mValue = hclamp(_calculateValue(mTimeSinceLastFrame), 0.0f, 255.0f);
			if ((unsigned char)mValue != mParent->getBlue())
			{
				mParent->setBlue((unsigned char)mValue);
			}
		}
		
	}
}

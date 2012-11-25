/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "AnimatorGreenChanger.h"

namespace aprilui
{
	namespace Animators
	{
		GreenChanger::GreenChanger(chstr name) : Animator(name)
		{
		}

		GreenChanger::~GreenChanger()
		{
		}

		Animator* GreenChanger::createInstance(chstr name)
		{
			return new GreenChanger(name);
		}

		void GreenChanger::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = mParent->getGreen();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void GreenChanger::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				mValue = hclamp(_calculateValue(mTimeSinceLastFrame), 0.0f, 255.0f);
				if ((unsigned char)mValue != mParent->getGreen())
				{
					mParent->setGreen((unsigned char)mValue);
				}
			}
		}
		
	}
}
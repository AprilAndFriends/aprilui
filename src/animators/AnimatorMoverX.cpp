/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "aprilui.h"
#include "AnimatorMoverX.h"

namespace aprilui
{
	namespace Animators
	{
		MoverX::MoverX(chstr name) : Animator(name)
		{
		}

		MoverX::~MoverX()
		{
		}

		Animator* MoverX::createInstance(chstr name)
		{
			return new MoverX(name);
		}

		void MoverX::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = mParent->getX();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void MoverX::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			mValue = mParent->getX();
			mValue = _calculateValue(mTimeSinceLastFrame);
			mParent->setX(mValue);
		}
		
	}
}
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
#include "AnimatorScalerX.h"

namespace aprilui
{
	namespace Animators
	{
		ScalerX::ScalerX(chstr name) : Animator(name)
		{
		}

		ScalerX::~ScalerX()
		{
		}

		Animator* ScalerX::createInstance(chstr name)
		{
			return new ScalerX(name);
		}

		void ScalerX::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = mParent->getScaleX();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void ScalerX::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			mValue = mParent->getScaleX();
			mValue = _calculateValue(mTimeSinceLastFrame);
			mParent->setScaleX(mValue);
		}
		
	}
}

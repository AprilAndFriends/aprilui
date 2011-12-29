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
#include "AnimatorResizerX.h"

namespace aprilui
{
	namespace Animators
	{
		ResizerX::ResizerX(chstr name) : Animator(name)
		{
		}

		ResizerX::~ResizerX()
		{
		}

		Animator* ResizerX::createInstance(chstr name)
		{
			return new ResizerX(name);
		}

		void ResizerX::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = mParent->getWidth();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void ResizerX::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			mValue = mParent->getWidth();
			mValue = _calculateValue(mTimeSinceLastFrame);
			mParent->setWidth(mValue);
		}
		
	}
}

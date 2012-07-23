/// @file
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "AnimatorResizerY.h"

namespace aprilui
{
	namespace Animators
	{
		ResizerY::ResizerY(chstr name) : Animator(name)
		{
		}

		ResizerY::~ResizerY()
		{
		}

		Animator* ResizerY::createInstance(chstr name)
		{
			return new ResizerY(name);
		}

		void ResizerY::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && this->mInheritValue)
			{
				this->mValue = this->mOffset = this->mParent->getHeight();
				if (this->mUseTarget)
				{
					this->mAmplitude = this->mTarget - this->mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void ResizerY::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				this->mValue = this->mParent->getHeight();
				this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
				this->mParent->setHeight(this->mValue);
			}
		}
		
	}
}

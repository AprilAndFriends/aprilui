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
			if (name == "AttachToObject" || name == "OnDelayEnd" && this->mInheritValue)
			{
				this->mValue = this->mOffset = this->mParent->getWidth();
				if (this->mUseTarget)
				{
					this->mAmplitude = this->mTarget - this->mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void ResizerX::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				this->mValue = this->mParent->getWidth();
				this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
				this->mParent->setWidth(this->mValue);
			}
		}
		
	}
}

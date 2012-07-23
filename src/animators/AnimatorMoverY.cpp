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
#include "AnimatorMoverY.h"

namespace aprilui
{
	namespace Animators
	{
		MoverY::MoverY(chstr name) : Animator(name)
		{
		}

		MoverY::~MoverY()
		{
		}

		Animator* MoverY::createInstance(chstr name)
		{
			return new MoverY(name);
		}

		void MoverY::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && this->mInheritValue)
			{
				this->mValue = this->mOffset = this->mParent->getY();
				if (this->mUseTarget)
				{
					this->mAmplitude = this->mTarget - this->mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void MoverY::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				this->mValue = this->mParent->getY();
				this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
				this->mParent->setY(this->mValue);
			}
		}
		
	}
}

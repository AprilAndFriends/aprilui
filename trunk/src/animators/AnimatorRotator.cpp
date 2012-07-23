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
#include "AnimatorRotator.h"

namespace aprilui
{
	namespace Animators
	{
		Rotator::Rotator(chstr name) : Animator(name)
		{
		}

		Rotator::~Rotator()
		{
		}

		Animator* Rotator::createInstance(chstr name)
		{
			return new Rotator(name);
		}

		void Rotator::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && this->mInheritValue)
			{
				this->mValue = this->mOffset = this->mParent->getAngle();
				if (this->mUseTarget)
				{
					this->mAmplitude = this->mTarget - this->mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void Rotator::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				this->mValue = this->mParent->getAngle();
				this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
				this->mParent->setAngle(this->mValue);
			}
		}
		
	}
}

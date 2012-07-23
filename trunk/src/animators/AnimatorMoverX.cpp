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
			if (name == "AttachToObject" || name == "OnDelayEnd" && this->mInheritValue)
			{
				this->mValue = this->mOffset = this->mParent->getX();
				if (this->mUseTarget)
				{
					this->mAmplitude = this->mTarget - this->mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void MoverX::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				this->mValue = this->mParent->getX();
				this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
				this->mParent->setX(this->mValue);
			}
		}
		
	}
}

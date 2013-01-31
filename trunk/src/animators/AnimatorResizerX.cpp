/// @file
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

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

		float ResizerX::_getObjectValue()
		{
			return this->mParent->getWidth();
		}

		void ResizerX::_setObjectValue(float value)
		{
			this->mParent->setWidth(value);
		}

		void ResizerX::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}

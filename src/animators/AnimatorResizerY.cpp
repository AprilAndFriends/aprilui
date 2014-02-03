/// @file
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

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

		float ResizerY::_getObjectValue()
		{
			return this->parent->getHeight();
		}

		void ResizerY::_setObjectValue(float value)
		{
			this->parent->setHeight(value);
		}

		void ResizerY::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}

/// @file
/// @version 3.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorResizerY.h"
#include "Object.h"

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

		void ResizerY::update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

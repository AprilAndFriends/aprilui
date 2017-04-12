/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorResizerX.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		ResizerX::ResizerX(chstr name) : Animator(name)
		{
		}

		ResizerX::ResizerX(const ResizerX& other) : Animator(other)
		{
		}

		ResizerX::~ResizerX()
		{
		}

		Animator* ResizerX::createInstance(chstr name)
		{
			return new ResizerX(name);
		}

		float ResizerX::_getObjectValue() const
		{
			return this->parent->getWidth();
		}

		void ResizerX::_setObjectValue(float value)
		{
			this->parent->setWidth(value);
		}

		void ResizerX::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorRotator.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		Rotator::Rotator(chstr name) : Animator(name)
		{
		}

		Rotator::Rotator(const Rotator& other) : Animator(other)
		{
		}

		Rotator::~Rotator()
		{
		}

		Animator* Rotator::createInstance(chstr name)
		{
			return new Rotator(name);
		}

		float Rotator::_getObjectValue() const
		{
			return this->parent->getAngle();
		}

		void Rotator::_setObjectValue(float value)
		{
			this->parent->setAngle(value);
		}

		void Rotator::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

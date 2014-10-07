/// @file
/// @version 3.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorMoverY.h"
#include "Object.h"

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

		float MoverY::_getObjectValue()
		{
			return this->parent->getY();
		}

		void MoverY::_setObjectValue(float value)
		{
			this->parent->setY(value);
		}

		void MoverY::update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

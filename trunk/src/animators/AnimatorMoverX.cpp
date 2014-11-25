/// @file
/// @version 3.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorMoverX.h"
#include "Object.h"

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

		float MoverX::_getObjectValue()
		{
			return this->parent->getX();
		}

		void MoverX::_setObjectValue(float value)
		{
			this->parent->setX(value);
		}

		void MoverX::update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

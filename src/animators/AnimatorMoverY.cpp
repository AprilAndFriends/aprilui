/// @file
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

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

		float MoverY::_getObjectValue()
		{
			return this->parent->getY();
		}

		void MoverY::_setObjectValue(float value)
		{
			this->parent->setY(value);
		}

		void MoverY::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}

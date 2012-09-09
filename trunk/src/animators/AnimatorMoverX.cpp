/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

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

		float MoverX::_getObjectValue()
		{
			return this->mParent->getX();
		}

		void MoverX::_setObjectValue(float value)
		{
			this->mParent->setX(value);
		}

		void MoverX::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}

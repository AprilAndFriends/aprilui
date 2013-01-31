/// @file
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

#include "AnimatorRotator.h"

namespace aprilui
{
	namespace Animators
	{
		Rotator::Rotator(chstr name) : Animator(name)
		{
		}

		Rotator::~Rotator()
		{
		}

		Animator* Rotator::createInstance(chstr name)
		{
			return new Rotator(name);
		}

		float Rotator::_getObjectValue()
		{
			return this->mParent->getAngle();
		}

		void Rotator::_setObjectValue(float value)
		{
			this->mParent->setAngle(value);
		}

		void Rotator::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}

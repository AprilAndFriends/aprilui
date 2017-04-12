/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorRedChanger.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		RedChanger::RedChanger(chstr name) : Animator(name)
		{
		}

		RedChanger::RedChanger(const RedChanger& other) : Animator(other)
		{
		}

		RedChanger::~RedChanger()
		{
		}

		Animator* RedChanger::createInstance(chstr name)
		{
			return new RedChanger(name);
		}

		float RedChanger::_getObjectValue() const
		{
			return (float)this->parent->getRed();
		}

		void RedChanger::_setObjectValue(float value)
		{
			this->parent->setRed((unsigned char)value);
		}

		void RedChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}

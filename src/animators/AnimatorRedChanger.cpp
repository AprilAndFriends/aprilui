/// @file
/// @version 4.0
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

		RedChanger::~RedChanger()
		{
		}

		Animator* RedChanger::createInstance(chstr name)
		{
			return new RedChanger(name);
		}

		float RedChanger::_getObjectValue()
		{
			return (float)this->parent->getRed();
		}

		void RedChanger::_setObjectValue(float value)
		{
			this->parent->setRed((unsigned char)value);
		}

		void RedChanger::update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}

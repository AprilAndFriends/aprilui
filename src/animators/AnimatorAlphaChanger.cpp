/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAlphaChanger.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaChanger::AlphaChanger(chstr name) : Animator(name)
		{
		}

		AlphaChanger::AlphaChanger(const AlphaChanger& other) : Animator(other)
		{
		}

		AlphaChanger::~AlphaChanger()
		{
		}

		Animator* AlphaChanger::createInstance(chstr name)
		{
			return new AlphaChanger(name);
		}

		float AlphaChanger::_getObjectValue() const
		{
			return (float)this->parent->getAlpha();
		}

		void AlphaChanger::_setObjectValue(float value)
		{
			this->parent->setAlpha((unsigned char)value);
		}

		void AlphaChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}

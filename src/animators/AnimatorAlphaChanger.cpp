/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAlphaChanger.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaChanger::AlphaChanger(chstr name) : Animator(name)
		{
		}

		AlphaChanger::~AlphaChanger()
		{
		}

		Animator* AlphaChanger::createInstance(chstr name)
		{
			return new AlphaChanger(name);
		}

		float AlphaChanger::_getObjectValue()
		{
			return (float)this->parent->getAlpha();
		}

		void AlphaChanger::_setObjectValue(float value)
		{
			this->parent->setAlpha((unsigned char)value);
		}

		void AlphaChanger::update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}

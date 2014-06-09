/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorGreenChanger.h"

namespace aprilui
{
	namespace Animators
	{
		GreenChanger::GreenChanger(chstr name) : Animator(name)
		{
		}

		GreenChanger::~GreenChanger()
		{
		}

		Animator* GreenChanger::createInstance(chstr name)
		{
			return new GreenChanger(name);
		}

		float GreenChanger::_getObjectValue()
		{
			return (float)this->parent->getGreen();
		}

		void GreenChanger::_setObjectValue(float value)
		{
			this->parent->setGreen((unsigned char)value);
		}

		void GreenChanger::update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}

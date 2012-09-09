/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
			return (float)this->mParent->getGreen();
		}

		void GreenChanger::_setObjectValue(float value)
		{
			this->mParent->setGreen((unsigned char)value);
		}

		void GreenChanger::update(float k)
		{
			this->_valueUpdateUChar(k);
		}
		
	}
}

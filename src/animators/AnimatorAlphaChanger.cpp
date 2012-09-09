/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
			return (float)this->mParent->getAlpha();
		}

		void AlphaChanger::_setObjectValue(float value)
		{
			this->mParent->setAlpha((unsigned char)value);
		}

		void AlphaChanger::update(float k)
		{
			this->_valueUpdateUChar(k);
		}
		
	}
}

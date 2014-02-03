/// @file
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

#include "AnimatorScalerX.h"

namespace aprilui
{
	namespace Animators
	{
		ScalerX::ScalerX(chstr name) : Animator(name)
		{
		}

		ScalerX::~ScalerX()
		{
		}

		Animator* ScalerX::createInstance(chstr name)
		{
			return new ScalerX(name);
		}

		float ScalerX::_getObjectValue()
		{
			return this->parent->getScaleX();
		}

		void ScalerX::_setObjectValue(float value)
		{
			this->parent->setScaleX(value);
		}

		void ScalerX::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}

/// @file
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

#include "AnimatorScalerY.h"

namespace aprilui
{
	namespace Animators
	{
		ScalerY::ScalerY(chstr name) : Animator(name)
		{
		}

		ScalerY::~ScalerY()
		{
		}

		Animator* ScalerY::createInstance(chstr name)
		{
			return new ScalerY(name);
		}

		float ScalerY::_getObjectValue()
		{
			return this->parent->getScaleY();
		}

		void ScalerY::_setObjectValue(float value)
		{
			this->parent->setScaleY(value);
		}

		void ScalerY::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}

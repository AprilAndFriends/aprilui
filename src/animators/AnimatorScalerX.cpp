/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorScalerX.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		ScalerX::ScalerX(chstr name) : Animator(name)
		{
		}

		ScalerX::ScalerX(const ScalerX& other) : Animator(other)
		{
		}

		ScalerX::~ScalerX()
		{
		}

		Animator* ScalerX::createInstance(chstr name)
		{
			return new ScalerX(name);
		}

		float ScalerX::_getObjectValue() const
		{
			return this->parent->getScaleX();
		}

		void ScalerX::_setObjectValue(float value)
		{
			this->parent->setScaleX(value);
		}

		void ScalerX::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

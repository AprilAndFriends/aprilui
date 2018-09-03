/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorPivotMoverX.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		PivotMoverX::PivotMoverX(chstr name) :
			Animator(name)
		{
		}

		PivotMoverX::PivotMoverX(const PivotMoverX& other) :
			Animator(other)
		{
		}

		Animator* PivotMoverX::createInstance(chstr name)
		{
			return new PivotMoverX(name);
		}

		float PivotMoverX::_getObjectValue() const
		{
			return this->parent->getPivotX();
		}

		void PivotMoverX::_setObjectValue(float value)
		{
			this->parent->setPivotX(value);
		}

		void PivotMoverX::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

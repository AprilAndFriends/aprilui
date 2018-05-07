/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorPivotMoverY.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		PivotMoverY::PivotMoverY(chstr name) : Animator(name)
		{
		}

		PivotMoverY::PivotMoverY(const PivotMoverY& other) : Animator(other)
		{
		}

		PivotMoverY::~PivotMoverY()
		{
		}

		Animator* PivotMoverY::createInstance(chstr name)
		{
			return new PivotMoverY(name);
		}

		float PivotMoverY::_getObjectValue() const
		{
			return this->parent->getPivotY();
		}

		void PivotMoverY::_setObjectValue(float value)
		{
			this->parent->setPivotY(value);
		}

		void PivotMoverY::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

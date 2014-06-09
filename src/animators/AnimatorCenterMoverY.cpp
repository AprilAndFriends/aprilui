/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorCenterMoverY.h"

namespace aprilui
{
	namespace Animators
	{
		CenterMoverY::CenterMoverY(chstr name) : Animator(name)
		{
		}

		CenterMoverY::~CenterMoverY()
		{
		}

		Animator* CenterMoverY::createInstance(chstr name)
		{
			return new CenterMoverY(name);
		}

		float CenterMoverY::_getObjectValue()
		{
			return this->parent->getCenterY();
		}

		void CenterMoverY::_setObjectValue(float value)
		{
			this->parent->setCenterY(value);
		}

		void CenterMoverY::update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

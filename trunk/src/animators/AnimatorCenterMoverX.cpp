/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorCenterMoverX.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		CenterMoverX::CenterMoverX(chstr name) : Animator(name)
		{
		}

		CenterMoverX::~CenterMoverX()
		{
		}

		Animator* CenterMoverX::createInstance(chstr name)
		{
			return new CenterMoverX(name);
		}

		float CenterMoverX::_getObjectValue()
		{
			return this->parent->getCenterX();
		}

		void CenterMoverX::_setObjectValue(float value)
		{
			this->parent->setCenterX(value);
		}

		void CenterMoverX::update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
			return this->mParent->getCenterY();
		}

		void CenterMoverY::_setObjectValue(float value)
		{
			this->mParent->setCenterY(value);
		}

		void CenterMoverY::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}

/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorZOrderChanger.h"
#include "Object.h"

namespace aprilui
{
	namespace Animators
	{
		ZOrderChanger::ZOrderChanger(chstr name) : Animator(name)
		{
		}

		ZOrderChanger::ZOrderChanger(const ZOrderChanger& other) : Animator(other)
		{
		}

		ZOrderChanger::~ZOrderChanger()
		{
		}

		Animator* ZOrderChanger::createInstance(chstr name)
		{
			return new ZOrderChanger(name);
		}

		float ZOrderChanger::_getObjectValue()
		{
			return (float)this->parent->getZOrder();
		}

		void ZOrderChanger::_setObjectValue(float value)
		{
			this->parent->setZOrder((int)value);
		}

		void ZOrderChanger::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}

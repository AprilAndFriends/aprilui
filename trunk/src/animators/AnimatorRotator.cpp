/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "aprilui.h"
#include "AnimatorRotator.h"

namespace aprilui
{
	namespace Animators
	{
		Rotator::Rotator(chstr name) : Animator("Animators::Rotator", name, grect(0, 0, 1, 1))
		{
			mDelay = 0.0f;
			mDestAngle = -1.0f;
		}

		void Rotator::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mValue = mDcOffset = mParent->getAngle();
			}
			Object::notifyEvent(name, params);
		}
		
		void Rotator::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			mValue = mParent->getAngle();
			mValue = _calculateValue(k);
			mParent->setAngle(mValue);
		}
		
	}
}

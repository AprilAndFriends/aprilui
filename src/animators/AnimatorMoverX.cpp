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

#include "AprilUI.h"
#include "AnimatorMoverX.h"

namespace AprilUI
{
	namespace Animators
	{
		MoverX::MoverX(chstr name) : Animator("Animators::MoverX", name, grect(0, 0, 1, 1))
		{
			mFunction = Linear;
			mPeriods = 1.0f;
		}

		bool MoverX::isAnimated()
		{
			if (!Animator::isAnimated())
			{
				return false;
			}
			return (mParent->getX() != mAmplitude);
		}

		void MoverX::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mDcOffset = mParent->getX();
			}
			Object::notifyEvent(name, params);
		}
		
		void MoverX::update(float k)
		{
			Animator::update(k);
			if (!this->isAnimated())
			{
				return;
			}
			float value = mParent->getX();
			value = _calculateValue(value);
			mParent->setX(value);
		}
		
	}
}

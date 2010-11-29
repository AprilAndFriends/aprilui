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
#include "AnimatorScalerX.h"

namespace AprilUI
{
	namespace Animators
	{
		ScalerX::ScalerX(chstr name) : Animator("Animators::ScalerX", name, grect(0, 0, 1, 1))
		{
			mFunction = Linear;
			mPeriods = 1.0f;
		}

		void ScalerX::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mDcOffset = mParent->getWidth();
			}
			Object::notifyEvent(name, params);
		}
		
		void ScalerX::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			float value = mParent->getWidth();
			value = _calculateValue(k, value);
			mParent->setWidth(value);
		}
		
	}
}

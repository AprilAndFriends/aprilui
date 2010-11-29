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
#include "AnimatorAlphaChanger.h"

namespace AprilUI
{
	namespace Animators
	{
		AlphaChanger::AlphaChanger(chstr name) : Animator("Animators::AlphaChanger", name, grect(0, 0, 1, 1))
		{
			mFunction = Linear;
			mPeriods = 1.0f;
			mDiscrete = false;
		}

		void AlphaChanger::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mDcOffset = mParent->getAlpha();
			}
			Object::notifyEvent(name, params);
		}
		
		void AlphaChanger::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			float value = mParent->getAlpha();
			value = _calculateValue(value);
			mParent->setAlpha(value);
		}
		
	}
}

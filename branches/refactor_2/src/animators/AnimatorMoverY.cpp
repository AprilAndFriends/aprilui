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
#include "AnimatorMoverY.h"

namespace AprilUI
{
	namespace Animators
	{
		MoverY::MoverY(chstr name) : Animator("Animators::MoverY", name, grect(0, 0, 1, 1))
		{
			mFunction = Linear;
		}

		void MoverY::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mDcOffset = mParent->getY();
			}
			Object::notifyEvent(name, params);
		}
		
		void MoverY::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			float value = mParent->getY();
			value = _calculateValue(k, value);
			mParent->setY(value);
		}
		
	}
}

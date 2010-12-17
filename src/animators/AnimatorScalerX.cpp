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
#include "AnimatorScalerX.h"

namespace aprilui
{
	namespace Animators
	{
		ScalerX::ScalerX(chstr name) : Animator("Animators::ScalerX", name, grect(0, 0, 1, 1))
		{
		}

		void ScalerX::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mValue = mDcOffset = mParent->getWidth();
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
			mValue = mParent->getWidth();
			mValue = _calculateValue(k);
			mParent->setWidth(mValue);
		}
		
	}
}

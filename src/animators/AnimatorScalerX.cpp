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
#include "ObjectVisualObject.h"

namespace aprilui
{
	namespace Animators
	{
		ScalerX::ScalerX(chstr name) : Animator("Animators::ScalerX", name)
		{
		}

		void ScalerX::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = dynamic_cast<VisualObject*>(mParent)->getWidth();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
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
			VisualObject* parent = dynamic_cast<VisualObject*>(mParent);
			if (parent == NULL)
			{
				aprilui::log("Animator: parent object not a subclass of Objects::VisualObject!");
				return;
			}
			mValue = parent->getWidth();
			mValue = _calculateValue(mTimeSinceLastFrame);
			parent->setWidth(mValue);
		}
		
	}
}

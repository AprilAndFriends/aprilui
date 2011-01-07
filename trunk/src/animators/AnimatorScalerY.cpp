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
#include "AnimatorScalerY.h"
#include "ObjectUi.h"

namespace aprilui
{
	namespace Animators
	{
		ScalerY::ScalerY(chstr name) : Animator("Animators::ScalerY", name)
		{
		}

		void ScalerY::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = dynamic_cast<ObjectUi*>(mParent)->getHeight();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void ScalerY::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			ObjectUi* parent = dynamic_cast<ObjectUi*>(mParent);
			if (parent == NULL)
			{
				aprilui::log("Animator: parent object not a subclass of Objects::ObjectUi!");
				return;
			}
			mValue = parent->getHeight();
			mValue = _calculateValue(mTimeSinceLastFrame);
			parent->setHeight(mValue);
		}
		
	}
}

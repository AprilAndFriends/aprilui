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
#include "AnimatorAlphaChanger.h"
#include "ObjectUi.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaChanger::AlphaChanger(chstr name) : Animator("Animators::AlphaChanger", name)
		{
		}

		void AlphaChanger::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = dynamic_cast<ObjectUi*>(mParent)->getAlpha();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
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
			ObjectUi* parent = dynamic_cast<ObjectUi*>(mParent);
			mValue = hclamp(_calculateValue(mTimeSinceLastFrame), 0.0f, 255.0f);
			if ((unsigned char)mValue != parent->getAlpha())
			{
				parent->setAlpha((unsigned char)mValue);
			}
		}
		
	}
}

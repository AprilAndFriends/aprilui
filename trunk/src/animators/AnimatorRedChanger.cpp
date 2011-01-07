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
#include "AnimatorRedChanger.h"
#include "ObjectVisualObject.h"

namespace aprilui
{
	namespace Animators
	{
		RedChanger::RedChanger(chstr name) : Animator("Animators::RedChanger", name)
		{
		}

		void RedChanger::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = dynamic_cast<VisualObject*>(mParent)->getRed();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void RedChanger::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			VisualObject* parent = dynamic_cast<VisualObject*>(mParent);
			mValue = hclamp(_calculateValue(mTimeSinceLastFrame), 0.0f, 255.0f);
			if ((unsigned char)mValue != parent->getRed())
			{
				parent->setRed((unsigned char)mValue);
			}
		}
		
	}
}

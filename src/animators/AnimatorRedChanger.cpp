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

namespace aprilui
{
	namespace Animators
	{
		RedChanger::RedChanger(chstr name) : Animator("Animators::RedChanger", name, grect(0, 0, 1, 1))
		{
		}

		void RedChanger::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "InheritValue")
			{
				mValue = mDcOffset = mParent->getRed();
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
			unsigned char value = mParent->getRed();
			mValue = hclamp(_calculateValue(k), 0.0f, 255.0f);
			if (value != (unsigned char)mValue)
			{
				mParent->setRed(mValue);
			}
		}
		
	}
}

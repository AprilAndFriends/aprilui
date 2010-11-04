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

#include "Animators.h"

namespace AprilUI
{
	namespace Animators
	{
		AlphaHover::AlphaHover(chstr name) : Animator("Animators::AlphaHover",name,grect(0,0,1,1))
		{
			mMin=mOwnerAlpha=0; mMax=1; mSpeed=1;
		}

		void AlphaHover::setProperty(chstr name,chstr value)
		{
			if      (name == "min") mMin=mOwnerAlpha=value;
			else if (name == "max") mMax=value;
			else if (name == "speed") mSpeed=value;
		}	

		void AlphaHover::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mParent->setAlpha(mOwnerAlpha);
			}
			Object::notifyEvent(event_name,params);
		}

		void AlphaHover::update(float k)
		{
			float direction=(mParent->isCursorInside()) ? 1 : -1;
			mOwnerAlpha+=k*mSpeed*direction;
			if (mOwnerAlpha < mMin) mOwnerAlpha=mMin;
			if (mOwnerAlpha > mMax) mOwnerAlpha=mMax;
			mParent->setAlpha(mOwnerAlpha);
		}
	}
}

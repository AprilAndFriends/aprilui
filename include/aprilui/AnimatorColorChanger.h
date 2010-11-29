/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_COLOR_CHANGER_H
#define APRILUI_COLOR_CHANGER_H

#include <april/RenderSystem.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace AprilUI
{
	namespace Animators
	{
		// note: only works on ColoredImageBoxes
		class AprilUIExport ColorChanger : public Animator
		{
		public:
			ColorChanger(chstr name);
			
			bool isAnimated();
			void setSpeed(float value) { mSpeed = value; }
			void setProperty(chstr name, chstr value);
			
			void update(float k);
			
		protected:
			April::Color mLow;
			April::Color mHigh;
			float mSpeed;
			
		};
	}
}

#endif
